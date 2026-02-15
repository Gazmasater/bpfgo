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

	IPPROTO_TCP = 6
	IPPROTO_UDP = 17

	EV_SENDTO   = 1
	EV_RECVFROM = 2
	EV_CONNECT  = 3
	EV_ACCEPT   = 4
	EV_BINDOK   = 20
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

type PendingConnect struct {
	Client   Proc
	ClientEp string
	ServerEp string
	Seen     time.Time
}

var (
	commMu    sync.RWMutex
	commCache = make(map[[32]int8]string)

	listenMu sync.Mutex
	listenBy = make(map[EndpKey]Proc, 4096)

	connMu sync.Mutex
	connBy = make(map[ConnKey]Proc, 16384)

	pendMu sync.Mutex
	pendBy = make(map[ConnKey]PendingConnect, 16384)
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
	case IPPROTO_TCP:
		return "TCP"
	case IPPROTO_UDP:
		return "UDP"
	default:
		return fmt.Sprintf("P%d", p)
	}
}

/* ===== IPv4 byte-order FIX (1.0.0.127 -> 127.0.0.1) ===== */

func ip4BytesFromU32Net(x uint32) (b [4]byte) {
	// IMPORTANT: unsafe-cast gives reversed numeric on little-endian, so restore bytes via LittleEndian.
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
	// wildcard fallback (0.0.0.0 / ::)
	var zero [16]byte
	k2 := EndpKey{Family: family, Port: port, IP: zero}
	listenMu.Lock()
	p2, ok2 := listenBy[k2]
	listenMu.Unlock()
	return p2, ok2
}

func saveConn(key ConnKey, client Proc) {
	client.Seen = time.Now()
	connMu.Lock()
	connBy[key] = client
	connMu.Unlock()
}

func lookupConn(key ConnKey) (Proc, bool) {
	connMu.Lock()
	p, ok := connBy[key]
	connMu.Unlock()
	return p, ok
}

func savePending(key ConnKey, pc PendingConnect) {
	pc.Seen = time.Now()
	pendMu.Lock()
	pendBy[key] = pc
	pendMu.Unlock()
}

func takePending(key ConnKey) (PendingConnect, bool) {
	pendMu.Lock()
	v, ok := pendBy[key]
	if ok {
		delete(pendBy, key)
	}
	pendMu.Unlock()
	return v, ok
}

func cleanupTTL(ttl time.Duration) {
	t := time.NewTicker(1 * time.Second)
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

		// если CONNECT так и не получил ACCEPT (удалённый сервер) — печатаем CONNECT с server=?
		pendMu.Lock()
		for k, v := range pendBy {
			if v.Seen.Before(cut) {
				fmt.Printf("TCP CONNECT client=%s  %s -> %s  server=?\n",
					v.Client.String(), v.ClientEp, v.ServerEp)
				delete(pendBy, k)
			}
		}
		pendMu.Unlock()
	}
}

/* ===== prints ===== */

func printTCPConnect(client Proc, clientEp, serverEp string, server Proc) {
	fmt.Printf("TCP CONNECT client=%s  %s -> %s  server=%s\n",
		client.String(), clientEp, serverEp, server.String())
}

func printTCPAccept(server Proc, serverEp, clientEp string, client Proc) {
	fmt.Printf("TCP ACCEPT  server=%s  %s -> %s  client=%s\n",
		server.String(), serverEp, clientEp, client.String())
}

func printUDP(kind string, p Proc, srcEp, dstEp string) {
	// kind = SENDTO / RECVFROM
	fmt.Printf("UDP %-7s pid=%s  %s -> %s\n", kind, p.String(), srcEp, dstEp)
}

/* ===== init/load ===== */

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
	go cleanupTTL(2 * time.Second)

	selfName := filepath.Base(os.Args[0])

	var links []link.Link
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

	// TCP roles
	attach("syscalls", "sys_enter_bind", objs.TraceBindEnter)
	attach("syscalls", "sys_exit_bind", objs.TraceBindExit)

	attach("syscalls", "sys_enter_connect", objs.TraceConnectEnter)
	attach("syscalls", "sys_exit_connect", objs.TraceConnectExit)

	attach("syscalls", "sys_enter_accept4", objs.TraceAccept4Enter)
	attach("syscalls", "sys_exit_accept4", objs.TraceAccept4Exit)
	attach("syscalls", "sys_enter_accept", objs.TraceAcceptEnter)
	attach("syscalls", "sys_exit_accept", objs.TraceAcceptExit)

	attach("syscalls", "sys_enter_close", objs.TraceCloseEnter)

	// UDP flow
	attach("syscalls", "sys_enter_sendto", objs.TraceSendtoEnter)
	attach("syscalls", "sys_exit_sendto", objs.TraceSendtoExit)
	attach("syscalls", "sys_enter_recvfrom", objs.TraceRecvfromEnter)
	attach("syscalls", "sys_exit_recvfrom", objs.TraceRecvfromExit)

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
			comm := commString(ev.Comm)
			if comm == selfName {
				continue
			}

			p := Proc{Pid: ev.Pid, Comm: comm}

			// endpoints
			var srcEp, dstEp string
			var srcIPKey, dstIPKey [16]byte
			var sport, dport uint16

			if ev.Family == AF_INET {
				sIP, sKey, sP := endpFromEvIPv4(ev.SrcIP.S_addr, ev.Sport)
				dIP, dKey, dP := endpFromEvIPv4(ev.DstIP.S_addr, ev.Dport)
				srcEp = formatEndp(AF_INET, sIP, sP)
				dstEp = formatEndp(AF_INET, dIP, dP)
				srcIPKey, dstIPKey = sKey, dKey
				sport, dport = sP, dP
			} else if ev.Family == AF_INET6 {
				sIP, sKey, sP := endpFromEvIPv6(ev.SrcIP6.In6U.U6Addr8, ev.Sport)
				dIP, dKey, dP := endpFromEvIPv6(ev.DstIP6.In6U.U6Addr8, ev.Dport)
				srcEp = formatEndp(AF_INET6, sIP, sP)
				dstEp = formatEndp(AF_INET6, dIP, dP)
				srcIPKey, dstIPKey = sKey, dKey
				sport, dport = sP, dP
			} else {
				continue
			}

			switch ev.Sysexit {

			case EV_BINDOK:
				// bind: сохраняем "кто слушает порт"
				saveListen(EndpKey{Family: uint16(ev.Family), Port: dport, IP: dstIPKey}, p)
				var zero [16]byte
				saveListen(EndpKey{Family: uint16(ev.Family), Port: dport, IP: zero}, p)

			case EV_CONNECT:
				// CONNECT (TCP): src=client, dst=server
				if uint8(ev.Proto) != IPPROTO_TCP {
					continue
				}
				key := ConnKey{
					Family:   uint16(ev.Family),
					ClientIP: srcIPKey, ClientPort: sport,
					ServerIP: dstIPKey, ServerPort: dport,
				}
				saveConn(key, p)

				// если сервер уже известен по bind — печатаем сразу
				if server, ok := lookupListen(uint16(ev.Family), dstIPKey, dport); ok {
					printTCPConnect(p, srcEp, dstEp, server)
				} else {
					// иначе ждём ACCEPT; если не дождёмся — cleanupTTL напечатает server=?
					savePending(key, PendingConnect{
						Client:   p,
						ClientEp: srcEp,
						ServerEp: dstEp,
					})
				}

			case EV_ACCEPT:
				// ACCEPT (TCP): событие в server pid, tuple src=client dst=server
				if uint8(ev.Proto) != IPPROTO_TCP {
					continue
				}
				server := p

				// на всякий — зафиксируем server для порта (если bind не поймали)
				saveListen(EndpKey{Family: uint16(ev.Family), Port: dport, IP: dstIPKey}, server)
				var zero [16]byte
				saveListen(EndpKey{Family: uint16(ev.Family), Port: dport, IP: zero}, server)

				key := ConnKey{
					Family:   uint16(ev.Family),
					ClientIP: srcIPKey, ClientPort: sport,
					ServerIP: dstIPKey, ServerPort: dport,
				}

				// если CONNECT был отложен — печатаем CONNECT уже с pid сервера
				if pc, ok := takePending(key); ok {
					printTCPConnect(pc.Client, pc.ClientEp, pc.ServerEp, server)
					printTCPAccept(server, pc.ServerEp, pc.ClientEp, pc.Client)
					continue
				}

				// иначе CONNECT мог быть напечатан уже (bind известен)
				client, ok := lookupConn(key)
				if !ok {
					client = Proc{}
				}
				printTCPAccept(server, dstEp, srcEp, client)

			case EV_SENDTO:
				// UDP sendto (и вообще sendto)
				if uint8(ev.Proto) == IPPROTO_UDP {
					printUDP("SENDTO", p, srcEp, dstEp)
				}

			case EV_RECVFROM:
				// UDP recvfrom
				if uint8(ev.Proto) == IPPROTO_UDP {
					printUDP("RECVFROM", p, srcEp, dstEp)
				}
			}
		}
	}()

	fmt.Println("Press Ctrl+C to exit")
	<-stop
	fmt.Println("Exiting...")
}
