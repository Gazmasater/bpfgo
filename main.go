package main

import (
	"bpfgo/pkg"
	"encoding/binary"
	"errors"
	"fmt"
	"log"
	"net"
	"net/http"
	_ "net/http/pprof"
	"os"
	"os/signal"
	"path/filepath"
	"sync"
	"syscall"
	"time"
	"unsafe"

	"github.com/cilium/ebpf/link"
	"github.com/cilium/ebpf/perf"
	"github.com/cilium/ebpf/rlimit"
)

var objs bpfObjects

const (
	AF_INET  = 2
	AF_INET6 = 10
)

type PeerKey struct {
	Family uint16
	Port   uint16
	IP     [16]byte // for IPv4: first 4 bytes used
}

type LookupInfo struct {
	Family uint16
	Proto  uint8

	PeerIP   [16]byte
	PeerPort uint16

	LocalIP   [16]byte
	LocalPort uint16

	Seen time.Time
}

var (
	lookupMu     sync.Mutex
	lookupByPeer = make(map[PeerKey]LookupInfo, 8192)
)

var (
	resolveCache = make(map[string]string)
	cacheMu      sync.RWMutex

	commCache = make(map[[32]int8]string)
	commMu    sync.RWMutex
)

func ip4FromBE(u uint32) net.IP {
	var b [4]byte
	binary.BigEndian.PutUint32(b[:], u)
	return net.IP(b[:])
}

func cachedComm(c [32]int8) string {
	commMu.RLock()
	if s, ok := commCache[c]; ok {
		commMu.RUnlock()
		return s
	}
	commMu.RUnlock()

	s := pkg.Int8ToString(c)

	commMu.Lock()
	commCache[c] = s
	commMu.Unlock()
	return s
}

func resolveHost(ip net.IP) string {
	key := ip.String()

	cacheMu.RLock()
	if host, ok := resolveCache[key]; ok {
		cacheMu.RUnlock()
		return host
	}
	cacheMu.RUnlock()

	var host string
	if ip.IsLoopback() {
		host = "localhost"
	} else {
		if ip.To4() != nil {
			host = pkg.ResolveIP(ip)
		} else {
			h, err := pkg.ResolveIP_n(ip)
			if err != nil {
				host = "unknown"
			} else {
				host = h
			}
		}
	}

	cacheMu.Lock()
	resolveCache[key] = host
	cacheMu.Unlock()
	return host
}

func ip6FromArr(a [16]uint8) net.IP {
	ip := make(net.IP, net.IPv6len)
	copy(ip, a[:])
	return ip
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

func saveLookupBothDirections(family uint16, proto uint8, srcIP [16]byte, srcPort uint16, dstIP [16]byte, dstPort uint16) {
	now := time.Now()

	// интерпретация #1: peer = src, local = dst
	k1 := PeerKey{Family: family, Port: srcPort, IP: srcIP}
	v1 := LookupInfo{
		Family:    family,
		Proto:     proto,
		PeerIP:    srcIP,
		PeerPort:  srcPort,
		LocalIP:   dstIP,
		LocalPort: dstPort,
		Seen:      now,
	}

	// интерпретация #2: peer = dst, local = src (на случай, если local/remote перепутаны в BPF)
	k2 := PeerKey{Family: family, Port: dstPort, IP: dstIP}
	v2 := LookupInfo{
		Family:    family,
		Proto:     proto,
		PeerIP:    dstIP,
		PeerPort:  dstPort,
		LocalIP:   srcIP,
		LocalPort: srcPort,
		Seen:      now,
	}

	lookupMu.Lock()
	lookupByPeer[k1] = v1
	lookupByPeer[k2] = v2
	lookupMu.Unlock()
}

func takeLookup(family uint16, peerIP [16]byte, peerPort uint16) (LookupInfo, bool) {
	k := PeerKey{Family: family, Port: peerPort, IP: peerIP}
	lookupMu.Lock()
	v, ok := lookupByPeer[k]
	lookupMu.Unlock()
	return v, ok
}

func cleanupLookups(ttl time.Duration) {
	t := time.NewTicker(2 * time.Second)
	defer t.Stop()

	for range t.C {
		cut := time.Now().Add(-ttl)
		lookupMu.Lock()
		for k, v := range lookupByPeer {
			if v.Seen.Before(cut) {
				delete(lookupByPeer, k)
			}
		}
		lookupMu.Unlock()
	}
}

func init() {
	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("failed to remove memory limit: %v", err)
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

	go cleanupLookups(3 * time.Second)

	netns, err := os.Open("/proc/self/ns/net")
	if err != nil {
		log.Fatalf("open netns: %v", err)
	}
	defer netns.Close()

	fmt.Printf("netns fd=%d\n", netns.Fd())
	fmt.Printf("sizeof(traceInfo)=%d\n", unsafe.Sizeof(bpfTraceInfo{}))

	links := make([]link.Link, 0, 16)
	defer func() {
		for _, l := range links {
			_ = l.Close()
		}
	}()

	BindEnter, err := link.Tracepoint("syscalls", "sys_enter_bind", objs.TraceBindEnter, nil)
	if err != nil {
		log.Fatalf("opening tracepoint sys_enter_bind: %s", err)
	}
	defer BindEnter.Close()

	BindExit, err := link.Tracepoint("syscalls", "sys_exit_bind", objs.TraceBindExit, nil)
	if err != nil {
		log.Fatalf("opening tracepoint sys_exit_bind: %s", err)
	}
	defer BindExit.Close()

	// --- connect ---
	ConnEnter, err := link.Tracepoint("syscalls", "sys_enter_connect", objs.TraceConnectEnter, nil)
	if err != nil {
		log.Fatalf("opening tracepoint sys_enter_connect: %s", err)
	}
	defer ConnEnter.Close()

	ConnExit, err := link.Tracepoint("syscalls", "sys_exit_connect", objs.TraceConnectExit, nil)
	if err != nil {
		log.Fatalf("opening tracepoint sys_exit_connect: %s", err)
	}
	defer ConnExit.Close()

	// --- accept4 ---
	Acc4Enter, err := link.Tracepoint("syscalls", "sys_enter_accept4", objs.TraceAccept4Enter, nil)
	if err != nil {
		log.Fatalf("opening tracepoint sys_enter_accept4: %s", err)
	}
	defer Acc4Enter.Close()

	Acc4Exit, err := link.Tracepoint("syscalls", "sys_exit_accept4", objs.TraceAccept4Exit, nil)
	if err != nil {
		log.Fatalf("opening tracepoint sys_exit_accept4: %s", err)
	}
	defer Acc4Exit.Close()

	// --- accept (если кто-то вызывает accept, а не accept4) ---
	AccEnter, err := link.Tracepoint("syscalls", "sys_enter_accept", objs.TraceAcceptEnter, nil)
	if err != nil {
		log.Fatalf("opening tracepoint sys_enter_accept: %s", err)
	}
	defer AccEnter.Close()

	AccExit, err := link.Tracepoint("syscalls", "sys_exit_accept", objs.TraceAcceptExit, nil)
	if err != nil {
		log.Fatalf("opening tracepoint sys_exit_accept: %s", err)
	}
	defer AccExit.Close()

	// --- close (чистим fd_state_map) ---
	CloseEnter, err := link.Tracepoint("syscalls", "sys_enter_close", objs.TraceCloseEnter, nil)
	if err != nil {
		log.Fatalf("opening tracepoint sys_enter_close: %s", err)
	}
	defer CloseEnter.Close()

	// --- Attach tracepoints correctly (two-value returns) ---
	{
		l, err := link.Tracepoint("syscalls", "sys_enter_sendmsg", objs.TraceSendmsgEnter, nil)
		if err != nil {
			log.Fatalf("attach sys_enter_sendmsg: %v", err)
		}
		links = append(links, l)
	}
	{
		l, err := link.Tracepoint("syscalls", "sys_exit_sendmsg", objs.TraceSendmsgExit, nil)
		if err != nil {
			log.Fatalf("attach sys_exit_sendmsg: %v", err)
		}
		links = append(links, l)
	}
	{
		l, err := link.Tracepoint("syscalls", "sys_enter_sendto", objs.TraceSendtoEnter, nil)
		if err != nil {
			log.Fatalf("attach sys_enter_sendto: %v", err)
		}
		links = append(links, l)
	}
	{
		l, err := link.Tracepoint("syscalls", "sys_exit_sendto", objs.TraceSendtoExit, nil)
		if err != nil {
			log.Fatalf("attach sys_exit_sendto: %v", err)
		}
		links = append(links, l)
	}
	{
		l, err := link.Tracepoint("syscalls", "sys_enter_recvmsg", objs.TraceRecvmsgEnter, nil)
		if err != nil {
			log.Fatalf("attach sys_enter_recvmsg: %v", err)
		}
		links = append(links, l)
	}
	{
		l, err := link.Tracepoint("syscalls", "sys_exit_recvmsg", objs.TraceRecvmsgExit, nil)
		if err != nil {
			log.Fatalf("attach sys_exit_recvmsg: %v", err)
		}
		links = append(links, l)
	}
	{
		l, err := link.Tracepoint("syscalls", "sys_enter_recvfrom", objs.TraceRecvfromEnter, nil)
		if err != nil {
			log.Fatalf("attach sys_enter_recvfrom: %v", err)
		}
		links = append(links, l)
	}
	{
		l, err := link.Tracepoint("syscalls", "sys_exit_recvfrom", objs.TraceRecvfromExit, nil)
		if err != nil {
			log.Fatalf("attach sys_exit_recvfrom: %v", err)
		}
		links = append(links, l)
	}

	stop := make(chan os.Signal, 1)
	signal.Notify(stop, os.Interrupt, syscall.SIGTERM)

	go func() {
		const buffLen = 256 * 1024
		rd, err := perf.NewReader(objs.TraceEvents, buffLen)
		if err != nil {
			log.Fatalf("perf.NewReader: %v", err)
		}
		defer rd.Close()

		selfName := filepath.Base(os.Args[0])

		for {
			record, err := rd.Read()
			if err != nil {
				if errors.Is(err, perf.ErrClosed) {
					return
				}
				log.Printf("perf read error: %v", err)
				continue
			}

			if record.LostSamples != 0 {
				log.Printf("LOST %d samples", record.LostSamples)
				continue
			}

			if len(record.RawSample) < int(unsafe.Sizeof(bpfTraceInfo{})) {
				log.Printf("invalid event size: %d", len(record.RawSample))
				continue
			}

			ev := *(*bpfTraceInfo)(unsafe.Pointer(&record.RawSample[0]))
			comm := cachedComm(ev.Comm)
			if comm == selfName {
				continue
			}

			switch ev.Sysexit {

			// sendto_exit
			case 1:
				if ev.Family == AF_INET {
					dst := ip4FromBE(ev.DstIP.S_addr)
					fmt.Printf("SENDTO  pid=%d comm=%s  %s dst=%s:%d\n",
						ev.Pid, comm, protoStr(uint8(ev.Proto)),
						dst.String(), ev.Dport)
				} else if ev.Family == AF_INET6 {
					dst := ip6FromArr(ev.DstIP6.In6U.U6Addr8)
					fmt.Printf("SENDTO6 pid=%d comm=%s  %s dst=[%s]:%d\n",
						ev.Pid, comm, protoStr(uint8(ev.Proto)),
						dst.String(), ev.Dport)
				}

			// recvfrom_exit
			case 2:
				handleRecv("RECVFROM", ev, comm)

			// recvmsg_exit
			case 12:
				handleRecv("RECVMSG", ev, comm)

			// sendmsg_exit
			case 11:
				if ev.Family == AF_INET {
					dst := ip4FromBE(ev.DstIP.S_addr)
					fmt.Printf("SENDMSG  pid=%d comm=%s  %s dst=%s:%d\n",
						ev.Pid, comm, protoStr(uint8(ev.Proto)),
						dst.String(), ev.Dport)
				} else if ev.Family == AF_INET6 {
					dst := ip6FromArr(ev.DstIP6.In6U.U6Addr8)
					fmt.Printf("SENDMSG6 pid=%d comm=%s  %s dst=[%s]:%d\n",
						ev.Pid, comm, protoStr(uint8(ev.Proto)),
						dst.String(), ev.Dport)
				}

			case 20:
				if ev.Family == 2 {
					ip := ip4FromBE(ev.DstIP.S_addr)
					fmt.Printf("BIND OK pid=%d comm=%s  %s:%d\n",
						ev.Pid, cachedComm(ev.Comm), ip.String(), ev.Dport)
				} else if ev.Family == 10 {
					// IPv6 печать как у тебя (через words/bytes)
					fmt.Printf("BIND6 OK pid=%d comm=%s  port=%d\n",
						ev.Pid, cachedComm(ev.Comm), ev.Dport)
				}

			// sk_lookup
			case 3:
				if ev.Family == AF_INET {
					var srcIP [16]byte
					var dstIP [16]byte
					binary.BigEndian.PutUint32(srcIP[:4], ev.SrcIP.S_addr)
					binary.BigEndian.PutUint32(dstIP[:4], ev.DstIP.S_addr)

					saveLookupBothDirections(
						uint16(ev.Family),
						uint8(ev.Proto),
						srcIP, uint16(ev.Sport),
						dstIP, uint16(ev.Dport),
					)
				} else if ev.Family == AF_INET6 {
					var srcIP [16]byte
					var dstIP [16]byte
					copy(srcIP[:], ev.SrcIP6.In6U.U6Addr8[:])
					copy(dstIP[:], ev.DstIP6.In6U.U6Addr8[:])

					saveLookupBothDirections(
						uint16(ev.Family),
						uint8(ev.Proto),
						srcIP, uint16(ev.Sport),
						dstIP, uint16(ev.Dport),
					)
				}

			// inet_sock_set_state (опционально)
			case 6:
				if ev.Family == AF_INET && ev.State != 0 {
					src := ip4FromBE(ev.SrcIP.S_addr)
					dst := ip4FromBE(ev.DstIP.S_addr)
					fmt.Printf("TCPSTATE pid=%d comm=%s state=%d %s:%d -> %s:%d\n",
						ev.Pid, comm, ev.State, src.String(), ev.Sport, dst.String(), ev.Dport)
				}
			}
		}
	}()

	fmt.Println("Press Ctrl+C to exit")
	<-stop
	fmt.Println("Exiting...")
}

func handleRecv(tag string, ev bpfTraceInfo, comm string) {
	if ev.Family == AF_INET {
		peerIPBE := ev.SrcIP.S_addr
		peer := ip4FromBE(peerIPBE)
		peerPort := uint16(ev.Sport)

		var peerKeyIP [16]byte
		binary.BigEndian.PutUint32(peerKeyIP[:4], peerIPBE)

		li, ok := takeLookup(uint16(AF_INET), peerKeyIP, peerPort)
		if ok && time.Since(li.Seen) <= 3*time.Second {
			local := net.IP(li.LocalIP[:4])
			p := protoStr(li.Proto)

			dstHost := resolveHost(local)
			srcHost := resolveHost(peer)

			fmt.Printf("%-7s pid=%d comm=%s  %s  %s[%s:%d] -> %s[%s:%d]\n",
				tag, ev.Pid, comm, p,
				srcHost, peer.String(), peerPort,
				dstHost, local.String(), li.LocalPort)
			return
		}

		fmt.Printf("%-7s pid=%d comm=%s  peer=%s:%d (no lookup)\n",
			tag, ev.Pid, comm, peer.String(), peerPort)
		return
	}

	if ev.Family == AF_INET6 {
		peer := ip6FromArr(ev.SrcIP6.In6U.U6Addr8)
		peerPort := uint16(ev.Sport)

		var peerKeyIP [16]byte
		copy(peerKeyIP[:], ev.SrcIP6.In6U.U6Addr8[:])

		li, ok := takeLookup(uint16(AF_INET6), peerKeyIP, peerPort)
		if ok && time.Since(li.Seen) <= 3*time.Second {
			local := net.IP(li.LocalIP[:16])
			p := protoStr(li.Proto)

			dstHost := resolveHost(local)
			srcHost := resolveHost(peer)

			fmt.Printf("%-7s pid=%d comm=%s  %s  %s[%s:%d] -> %s[%s:%d]\n",
				tag, ev.Pid, comm, p,
				srcHost, peer.String(), peerPort,
				dstHost, local.String(), li.LocalPort)
			return
		}

		fmt.Printf("%-7s pid=%d comm=%s  peer=[%s]:%d (no lookup)\n",
			tag, ev.Pid, comm, peer.String(), peerPort)
	}
}
