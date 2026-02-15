package main

import (
	"bytes"
	"encoding/binary"
	"errors"
	"fmt"
	"log"
	"net/http"
	_ "net/http/pprof"
	"os"
	"os/signal"
	"path/filepath"
	"sync"
	"syscall"
	"time"
	"unsafe"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/link"
	"github.com/cilium/ebpf/perf"
	"github.com/cilium/ebpf/rlimit"
)

var objs bpfObjects

const (
	AF_INET  = 2
	AF_INET6 = 10

	EV_CONNECT = 3
	EV_ACCEPT  = 4
	EV_BINDOK  = 20

	IPPROTO_TCP = 6
)

type Proc struct {
	Pid  uint32
	Comm string
	Seen time.Time
}

func (p Proc) String() string {
	if p.Pid == 0 {
		return "?"
	}
	if p.Comm == "" {
		return fmt.Sprintf("%d(?)", p.Pid)
	}
	return fmt.Sprintf("%d(%s)", p.Pid, p.Comm)
}

/* ===== keys/caches ===== */

type EndpKey struct {
	Family uint16
	Port   uint16
	IP     [16]byte
}

type ConnKey struct {
	Family     uint16
	ClientIP   [16]byte
	ClientPort uint16
	ServerIP   [16]byte
	ServerPort uint16
}

var (
	sepLine = "------------------------------------------------------------"

	commMu    sync.RWMutex
	commCache = make(map[[32]int8]string)

	listenMu sync.Mutex
	listenBy = make(map[EndpKey]Proc, 4096)

	connMu sync.Mutex
	connBy = make(map[ConnKey]Proc, 16384)
)

/* ===== comm/proto ===== */

func commString(c [32]int8) string {
	commMu.RLock()
	if s, ok := commCache[c]; ok {
		commMu.RUnlock()
		return s
	}
	commMu.RUnlock()

	var b [32]byte
	for i := 0; i < 32; i++ {
		b[i] = byte(c[i])
	}

	// найти первый '\0'
	n := bytes.IndexByte(b[:], 0)
	if n < 0 {
		n = len(b)
	}

	s := string(b[:n])

	commMu.Lock()
	commCache[c] = s
	commMu.Unlock()
	return s
}

func protoStr(p uint8) string {
	switch p {
	case 6:
		return "TCP"
	case 17:
		return "UDP"
	default:
		return fmt.Sprintf("P%d", p)
	}
}

/* ===== IPv4 byte-order FIX =====
BPF writes IPv4 as __be32; when Go unsafe-casts to uint32 on little-endian,
the bytes appear reversed in number form. Rebuild bytes using LittleEndian.
*/

func ip4BytesFromU32Net(x uint32) (b [4]byte) {
	binary.LittleEndian.PutUint32(b[:], x)
	return
}

func endpFromEvIPv4(ipU32 uint32, port uint16) (ipStr string, ipKey [16]byte, portU16 uint16) {
	b := ip4BytesFromU32Net(ipU32)
	copy(ipKey[:4], b[:])
	return fmt.Sprintf("%d.%d.%d.%d", b[0], b[1], b[2], b[3]), ipKey, port
}

func endpFromEvIPv6(ipArr [16]uint8, port uint16) (ipStr string, ipKey [16]byte, portU16 uint16) {
	copy(ipKey[:], ipArr[:])
	// минимально красиво (без net.IP stringify, чтобы без аллокаций)
	return fmt.Sprintf("%x:%x:%x:%x:%x:%x:%x:%x",
		uint16(ipArr[0])<<8|uint16(ipArr[1]),
		uint16(ipArr[2])<<8|uint16(ipArr[3]),
		uint16(ipArr[4])<<8|uint16(ipArr[5]),
		uint16(ipArr[6])<<8|uint16(ipArr[7]),
		uint16(ipArr[8])<<8|uint16(ipArr[9]),
		uint16(ipArr[10])<<8|uint16(ipArr[11]),
		uint16(ipArr[12])<<8|uint16(ipArr[13]),
		uint16(ipArr[14])<<8|uint16(ipArr[15]),
	), ipKey, port
}

func formatEndp(family uint16, ipStr string, port uint16) string {
	if family == AF_INET6 {
		return fmt.Sprintf("[%s]:%d", ipStr, port)
	}
	return fmt.Sprintf("%s:%d", ipStr, port)
}

/* ===== caches ===== */

func saveListen(ep EndpKey, p Proc) {
	p.Seen = time.Now()
	listenMu.Lock()
	listenBy[ep] = p
	listenMu.Unlock()
}

func lookupListen(family uint16, ip [16]byte, port uint16) (Proc, bool) {
	k := EndpKey{Family: family, Port: port, IP: ip}
	listenMu.Lock()
	p, ok := listenBy[k]
	listenMu.Unlock()
	if ok {
		return p, true
	}
	var zero [16]byte
	k2 := EndpKey{Family: family, Port: port, IP: zero}
	listenMu.Lock()
	p2, ok2 := listenBy[k2]
	listenMu.Unlock()
	return p2, ok2
}

func saveConnectTuple(key ConnKey, client Proc) {
	client.Seen = time.Now()
	connMu.Lock()
	connBy[key] = client
	connMu.Unlock()
}

func lookupConnectTuple(key ConnKey) (Proc, bool) {
	connMu.Lock()
	p, ok := connBy[key]
	connMu.Unlock()
	return p, ok
}

func cleanupTTL(ttl time.Duration) {
	t := time.NewTicker(2 * time.Second)
	defer t.Stop()

	for range t.C {
		cut := time.Now().Add(-ttl)

		listenMu.Lock()
		for k, v := range listenBy {
			if v.Seen.Before(cut) {
				delete(listenBy, k)
			}
		}
		listenMu.Unlock()

		connMu.Lock()
		for k, v := range connBy {
			if v.Seen.Before(cut) {
				delete(connBy, k)
			}
		}
		connMu.Unlock()
	}
}

/* ===== pretty print ===== */

func printBlock(proto, kind string, client Proc, server Proc, clientEp, serverEp string) {
	fmt.Println(sepLine)
	fmt.Printf("%-4s %-7s client=%s  %s -> %s  server=%s\n",
		proto, kind, client.String(), clientEp, serverEp, server.String())
	fmt.Printf("%-4s %-7s server=%s  %s -> %s  client=%s\n",
		proto, kind, server.String(), serverEp, clientEp, client.String())
}

func init() {
	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("failed to remove memlock: %v", err)
	}
	if err := loadBpfObjects(&objs, nil); err != nil {
		log.Fatalf("failed to load bpf objects: %v", err)
	}
}

func main() {
	go func() {
		log.Println("pprof on :6060")
		_ = http.ListenAndServe(":6060", nil)
	}()

	defer objs.Close()
	go cleanupTTL(5 * time.Second)

	selfName := filepath.Base(os.Args[0])

	links := make([]link.Link, 0, 16)
	defer func() {
		for _, l := range links {
			_ = l.Close()
		}
	}()

	attach := func(cat, name string, prog *ebpf.Program) {
		l, err := link.Tracepoint(cat, name, prog, nil)
		if err != nil {
			log.Fatalf("attach %s/%s: %v", cat, name, err)
		}
		links = append(links, l)
	}

	// bind/connect/accept/close
	attach("syscalls", "sys_enter_bind", objs.TraceBindEnter)
	attach("syscalls", "sys_exit_bind", objs.TraceBindExit)

	attach("syscalls", "sys_enter_connect", objs.TraceConnectEnter)
	attach("syscalls", "sys_exit_connect", objs.TraceConnectExit)

	attach("syscalls", "sys_enter_accept4", objs.TraceAccept4Enter)
	attach("syscalls", "sys_exit_accept4", objs.TraceAccept4Exit)
	attach("syscalls", "sys_enter_accept", objs.TraceAcceptEnter)
	attach("syscalls", "sys_exit_accept", objs.TraceAcceptExit)

	attach("syscalls", "sys_enter_close", objs.TraceCloseEnter)

	stop := make(chan os.Signal, 1)
	signal.Notify(stop, os.Interrupt, syscall.SIGTERM)

	go func() {
		const buffLen = 256 * 1024
		rd, err := perf.NewReader(objs.TraceEvents, buffLen)
		if err != nil {
			log.Fatalf("perf.NewReader: %v", err)
		}
		defer rd.Close()

		for {
			rec, err := rd.Read()
			if err != nil {
				if errors.Is(err, perf.ErrClosed) {
					return
				}
				log.Printf("perf read error: %v", err)
				continue
			}
			if rec.LostSamples != 0 {
				log.Printf("LOST %d samples", rec.LostSamples)
				continue
			}
			if len(rec.RawSample) < int(unsafe.Sizeof(bpfTraceInfo{})) {
				continue
			}

			ev := *(*bpfTraceInfo)(unsafe.Pointer(&rec.RawSample[0]))

			// TCP only (как ты просил)
			if uint8(ev.Proto) != IPPROTO_TCP {
				continue
			}

			comm := commString(ev.Comm)
			if comm == selfName {
				continue
			}

			proto := protoStr(uint8(ev.Proto))

			switch ev.Sysexit {

			case EV_BINDOK:
				server := Proc{Pid: ev.Pid, Comm: comm}

				if ev.Family == AF_INET {
					_, ipKey, port := endpFromEvIPv4(ev.DstIP.S_addr, ev.Dport)
					ep := EndpKey{Family: uint16(ev.Family), Port: port, IP: ipKey}
					saveListen(ep, server)
					var zero [16]byte
					saveListen(EndpKey{Family: uint16(ev.Family), Port: port, IP: zero}, server)
				} else if ev.Family == AF_INET6 {
					_, ipKey, port := endpFromEvIPv6(ev.DstIP6.In6U.U6Addr8, ev.Dport)
					ep := EndpKey{Family: uint16(ev.Family), Port: port, IP: ipKey}
					saveListen(ep, server)
					var zero [16]byte
					saveListen(EndpKey{Family: uint16(ev.Family), Port: port, IP: zero}, server)
				}

			case EV_CONNECT:
				// CONNECT приходит из client pid, tuple = local->remote
				client := Proc{Pid: ev.Pid, Comm: comm}

				if ev.Family == AF_INET {
					cIPStr, cIPKey, cPort := endpFromEvIPv4(ev.SrcIP.S_addr, ev.Sport)
					sIPStr, sIPKey, sPort := endpFromEvIPv4(ev.DstIP.S_addr, ev.Dport)

					clientEp := formatEndp(AF_INET, cIPStr, cPort)
					serverEp := formatEndp(AF_INET, sIPStr, sPort)

					key := ConnKey{
						Family:   uint16(ev.Family),
						ClientIP: cIPKey, ClientPort: cPort,
						ServerIP: sIPKey, ServerPort: sPort,
					}
					saveConnectTuple(key, client)

					server, ok := lookupListen(uint16(ev.Family), sIPKey, sPort)
					if !ok {
						server = Proc{}
					}

					printBlock(proto, "CONNECT", client, server, clientEp, serverEp)

				} else if ev.Family == AF_INET6 {
					cIPStr, cIPKey, cPort := endpFromEvIPv6(ev.SrcIP6.In6U.U6Addr8, ev.Sport)
					sIPStr, sIPKey, sPort := endpFromEvIPv6(ev.DstIP6.In6U.U6Addr8, ev.Dport)

					clientEp := formatEndp(AF_INET6, cIPStr, cPort)
					serverEp := formatEndp(AF_INET6, sIPStr, sPort)

					key := ConnKey{
						Family:   uint16(ev.Family),
						ClientIP: cIPKey, ClientPort: cPort,
						ServerIP: sIPKey, ServerPort: sPort,
					}
					saveConnectTuple(key, client)

					server, ok := lookupListen(uint16(ev.Family), sIPKey, sPort)
					if !ok {
						server = Proc{}
					}

					printBlock(proto, "CONNECT", client, server, clientEp, serverEp)
				}

			case EV_ACCEPT:
				// ACCEPT приходит из server pid, tuple = client->server (мы так в BPF сделали)
				server := Proc{Pid: ev.Pid, Comm: comm}

				if ev.Family == AF_INET {
					cIPStr, cIPKey, cPort := endpFromEvIPv4(ev.SrcIP.S_addr, ev.Sport)
					sIPStr, sIPKey, sPort := endpFromEvIPv4(ev.DstIP.S_addr, ev.Dport)

					clientEp := formatEndp(AF_INET, cIPStr, cPort)
					serverEp := formatEndp(AF_INET, sIPStr, sPort)

					// accept может заменить bind-роль (если bind не увидели)
					saveListen(EndpKey{Family: uint16(ev.Family), Port: sPort, IP: sIPKey}, server)
					var zero [16]byte
					saveListen(EndpKey{Family: uint16(ev.Family), Port: sPort, IP: zero}, server)

					key := ConnKey{
						Family:   uint16(ev.Family),
						ClientIP: cIPKey, ClientPort: cPort,
						ServerIP: sIPKey, ServerPort: sPort,
					}
					client, ok := lookupConnectTuple(key)
					if !ok {
						client = Proc{}
					}

					printBlock(proto, "ACCEPT", client, server, clientEp, serverEp)

				} else if ev.Family == AF_INET6 {
					cIPStr, cIPKey, cPort := endpFromEvIPv6(ev.SrcIP6.In6U.U6Addr8, ev.Sport)
					sIPStr, sIPKey, sPort := endpFromEvIPv6(ev.DstIP6.In6U.U6Addr8, ev.Dport)

					clientEp := formatEndp(AF_INET6, cIPStr, cPort)
					serverEp := formatEndp(AF_INET6, sIPStr, sPort)

					saveListen(EndpKey{Family: uint16(ev.Family), Port: sPort, IP: sIPKey}, server)
					var zero [16]byte
					saveListen(EndpKey{Family: uint16(ev.Family), Port: sPort, IP: zero}, server)

					key := ConnKey{
						Family:   uint16(ev.Family),
						ClientIP: cIPKey, ClientPort: cPort,
						ServerIP: sIPKey, ServerPort: sPort,
					}
					client, ok := lookupConnectTuple(key)
					if !ok {
						client = Proc{}
					}

					printBlock(proto, "ACCEPT", client, server, clientEp, serverEp)
				}
			}
		}
	}()

	fmt.Println("Press Ctrl+C to exit")
	<-stop
	fmt.Println("Exiting...")
}
