package main

import (
	"bytes"
	"encoding/binary"
	"errors"
	"flag"
	"fmt"
	"log"
	"net"
	"net/http"
	_ "net/http/pprof"
	"os"
	"os/signal"
	"path/filepath"
	"strings"
	"sync/atomic"
	"syscall"
	"time"
	"unsafe"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/link"
	"github.com/cilium/ebpf/perf"
	"github.com/cilium/ebpf/rlimit"
)

//go:generate go run github.com/cilium/ebpf/cmd/bpf2go -target amd64 -type trace_info bpf trace.c -- -I.

var objs bpfObjects

const (
	AF_INET  = 2
	AF_INET6 = 10

	IPPROTO_ICMP   = 1
	IPPROTO_TCP    = 6
	IPPROTO_UDP    = 17
	IPPROTO_ICMPV6 = 58

	EV_SENDTO   = 1
	EV_RECVFROM = 2
	EV_CONNECT  = 3
	EV_ACCEPT   = 4
	EV_BINDOK   = 20
	EV_SENDMSG  = 11
	EV_RECVMSG  = 12

	EV_SENDMMSG = 13
	EV_RECVMMSG = 14
	EV_READ     = 21
	EV_WRITE    = 22

	EV_CLOSE = 30
)

var (
	flgPerfMB    = flag.Int("perfMB", 16, "perf buffer size in MB")
	flgPprof     = flag.Bool("pprof", true, "enable pprof")
	flgPprofAddr = flag.String("pprofAddr", ":6060", "pprof listen addr")

	flgTTL   = flag.Duration("ttl", 5*time.Second, "idle TTL for flow close")
	flgSweep = flag.Duration("print", 1*time.Second, "TTL sweep interval + perf-loss rate logging interval")

	flgOnlyPID  = flag.Int("pid", 0, "only this pid (0=all)")
	flgOnlyComm = flag.String("comm", "", "only comm containing substring (empty=all)")
)

func commString(c [32]int8) string {
	var b [32]byte
	for i := 0; i < 32; i++ {
		b[i] = byte(c[i])
	}
	n := bytes.IndexByte(b[:], 0)
	if n < 0 {
		n = len(b)
	}
	return string(b[:n])
}

func protoAllowed(p uint8) bool {
	switch p {
	case IPPROTO_TCP, IPPROTO_UDP, IPPROTO_ICMP, IPPROTO_ICMPV6:
		return true
	default:
		return false
	}
}

func protoStr(p uint8) string {
	switch p {
	case IPPROTO_TCP:
		return "TCP"
	case IPPROTO_UDP:
		return "UDP"
	case IPPROTO_ICMP:
		return "ICMP"
	case IPPROTO_ICMPV6:
		return "ICMPv6"
	default:
		return fmt.Sprintf("P%d", p)
	}
}

func isSend(ev uint8) bool {
	return ev == EV_SENDTO || ev == EV_SENDMSG || ev == EV_SENDMMSG || ev == EV_WRITE
}

func isRecv(ev uint8) bool {
	return ev == EV_RECVFROM || ev == EV_RECVMSG || ev == EV_RECVMMSG || ev == EV_READ
}

func isAllZero16(b [16]byte) bool {
	for i := 0; i < 16; i++ {
		if b[i] != 0 {
			return false
		}
	}
	return true
}

func ip4KeyFromU32Net(x uint32) (key [16]byte) {
	var b4 [4]byte
	binary.LittleEndian.PutUint32(b4[:], x)
	copy(key[:4], b4[:])
	return
}

func fmtIPv4FromKey(k [16]byte) string {
	return fmt.Sprintf("%d.%d.%d.%d", k[0], k[1], k[2], k[3])
}

func fmtIPv6Full(b [16]byte) string {
	return fmt.Sprintf("%x:%x:%x:%x:%x:%x:%x:%x",
		uint16(b[0])<<8|uint16(b[1]),
		uint16(b[2])<<8|uint16(b[3]),
		uint16(b[4])<<8|uint16(b[5]),
		uint16(b[6])<<8|uint16(b[7]),
		uint16(b[8])<<8|uint16(b[9]),
		uint16(b[10])<<8|uint16(b[11]),
		uint16(b[12])<<8|uint16(b[13]),
		uint16(b[14])<<8|uint16(b[15]),
	)
}

/* ===== scope only for ICMPv6 link-local ===== */

func isIPv6LinkLocal(ip [16]byte) bool {
	return ip[0] == 0xfe && (ip[1]&0xc0) == 0x80 // fe80::/10
}

var ifNameCache = map[uint32]string{}

func ifName(idx uint32) string {
	if idx == 0 {
		return ""
	}
	if s, ok := ifNameCache[idx]; ok {
		return s
	}
	ifi, err := net.InterfaceByIndex(int(idx))
	if err == nil && ifi != nil && ifi.Name != "" {
		ifNameCache[idx] = ifi.Name
		return ifi.Name
	}
	s := fmt.Sprintf("%d", idx)
	ifNameCache[idx] = s
	return s
}

func fmtIPv6WithScope(ip [16]byte, scope uint32) string {
	s := fmtIPv6Full(ip)
	if isIPv6LinkLocal(ip) && scope != 0 {
		return s + "%" + ifName(scope)
	}
	return s
}

func fmtEndpoint(family uint16, ip [16]byte, scope uint32, port uint16, proto uint8) string {
	isICMP := proto == IPPROTO_ICMP || proto == IPPROTO_ICMPV6
	if isICMP {
		if isAllZero16(ip) {
			return "*"
		}
		if family == AF_INET6 {
			if proto == IPPROTO_ICMPV6 {
				return fmtIPv6WithScope(ip, scope)
			}
			return fmtIPv6Full(ip)
		}
		return fmtIPv4FromKey(ip)
	}

	if isAllZero16(ip) {
		return fmt.Sprintf("*:%d", port)
	}
	if family == AF_INET6 {
		return fmt.Sprintf("[%s]:%d", fmtIPv6Full(ip), port)
	}
	return fmt.Sprintf("%s:%d", fmtIPv4FromKey(ip), port)
}

func srcKeyFromEvent(ev bpfTraceInfo) (k [16]byte) {
	if uint16(ev.Family) == AF_INET {
		return ip4KeyFromU32Net(ev.SrcIp4)
	}
	copy(k[:], ev.SrcIp6[:])
	return
}

func dstKeyFromEvent(ev bpfTraceInfo) (k [16]byte) {
	if uint16(ev.Family) == AF_INET {
		return ip4KeyFromU32Net(ev.DstIp4)
	}
	copy(k[:], ev.DstIp6[:])
	return
}

/* ===== FLOW ===== */

type FlowKey struct {
	TGID   uint32
	Cookie uint64
	Proto  uint8
	Family uint16

	PeerMode uint8 // 0=socket-only, 1=per-peer (UDP/ICMP when enough info)
	Rport    uint16
	Remote   [16]byte

	// only to disambiguate ICMPv6 link-local peers (fe80 needs ifindex)
	RemoteScope uint32
}

type Flow struct {
	Key  FlowKey
	Comm string

	Local  [16]byte
	Lport  uint16
	Remote [16]byte
	Rport  uint16

	LocalScope  uint32
	RemoteScope uint32

	FirstSeen time.Time
	LastSeen  time.Time

	InBytes  uint64
	OutBytes uint64
	InPkts   uint64
	OutPkts  uint64

	OpenedPrinted bool
	GenStart      uint64
}

func makeKey(ev bpfTraceInfo) FlowKey {
	k := FlowKey{
		TGID:   ev.Tgid,
		Cookie: ev.Cookie,
		Proto:  uint8(ev.Proto),
		Family: uint16(ev.Family),
	}

	switch k.Proto {
	case IPPROTO_UDP:
		var remote [16]byte
		var rport uint16
		if isRecv(uint8(ev.Event)) {
			remote = srcKeyFromEvent(ev)
			rport = uint16(ev.Sport)
		} else {
			remote = dstKeyFromEvent(ev)
			rport = uint16(ev.Dport)
		}
		if rport != 0 && !isAllZero16(remote) {
			k.PeerMode = 1
			k.Remote = remote
			k.Rport = rport
		}

	case IPPROTO_ICMP, IPPROTO_ICMPV6:
		var remote [16]byte
		var rscope uint32
		if isRecv(uint8(ev.Event)) {
			remote = srcKeyFromEvent(ev)
			rscope = uint32(ev.SrcScope)
		} else {
			remote = dstKeyFromEvent(ev)
			rscope = uint32(ev.DstScope)
		}
		if !isAllZero16(remote) {
			k.PeerMode = 1
			k.Remote = remote
			if k.Proto == IPPROTO_ICMPV6 && isIPv6LinkLocal(remote) && rscope != 0 {
				k.RemoteScope = rscope
			}
		}
	}

	return k
}

func applyEndpoints(f *Flow, ev bpfTraceInfo) {
	evt := uint8(ev.Event)
	proto := uint8(ev.Proto)

	var localIP, remoteIP [16]byte
	var lport, rport uint16

	var lscope, rscope uint32
	if proto == IPPROTO_ICMPV6 {
		srcScope := uint32(ev.SrcScope)
		dstScope := uint32(ev.DstScope)

		switch {
		case isSend(evt) || evt == EV_CONNECT:
			lscope = srcScope
			rscope = dstScope
		case isRecv(evt):
			lscope = dstScope
			rscope = srcScope
		case evt == EV_BINDOK:
			lscope = srcScope
		case evt == EV_ACCEPT:
			lscope = dstScope
			rscope = srcScope
		}
	}

	switch {
	case isSend(evt) || evt == EV_CONNECT:
		localIP = srcKeyFromEvent(ev)
		remoteIP = dstKeyFromEvent(ev)
		lport = uint16(ev.Sport)
		rport = uint16(ev.Dport)

	case isRecv(evt):
		localIP = dstKeyFromEvent(ev)
		remoteIP = srcKeyFromEvent(ev)
		lport = uint16(ev.Dport)
		rport = uint16(ev.Sport)

	case evt == EV_BINDOK:
		localIP = srcKeyFromEvent(ev)
		lport = uint16(ev.Sport)

	case evt == EV_ACCEPT:
		localIP = dstKeyFromEvent(ev)
		remoteIP = srcKeyFromEvent(ev)
		lport = uint16(ev.Dport)
		rport = uint16(ev.Sport)
	}

	if f.Lport == 0 && lport != 0 {
		f.Lport = lport
	}
	if isAllZero16(f.Local) && !isAllZero16(localIP) {
		f.Local = localIP
	}
	if f.Rport == 0 && rport != 0 {
		f.Rport = rport
	}
	if isAllZero16(f.Remote) && !isAllZero16(remoteIP) {
		f.Remote = remoteIP
	}

	if proto == IPPROTO_ICMPV6 {
		if f.LocalScope == 0 && lscope != 0 {
			f.LocalScope = lscope
		}
		if f.RemoteScope == 0 && rscope != 0 {
			f.RemoteScope = rscope
		}
	}
}

func flowReadyToOpen(f *Flow) bool {
	if isAllZero16(f.Remote) {
		return false
	}
	switch f.Key.Proto {
	case IPPROTO_TCP, IPPROTO_UDP:
		return f.Lport != 0 && f.Rport != 0
	default:
		return true
	}
}

var lostTotal uint64
var lostGen uint64

func maybeLostNote(f *Flow) string {
	if f.InBytes == 0 && f.OutBytes == 0 && f.GenStart != atomic.LoadUint64(&lostGen) {
		return " maybe_lost=1"
	}
	return ""
}

func incompleteNote(f *Flow) string {
	switch f.Key.Proto {
	case IPPROTO_TCP, IPPROTO_UDP:
		if isAllZero16(f.Remote) || f.Lport == 0 || f.Rport == 0 {
			return " incomplete=1"
		}
	case IPPROTO_ICMP, IPPROTO_ICMPV6:
		if isAllZero16(f.Remote) {
			return " incomplete=1"
		}
	}
	return ""
}

func dropZeroFlow(f *Flow) bool {
	if f.InBytes != 0 || f.OutBytes != 0 {
		return false
	}
	if f.Key.Proto == IPPROTO_UDP || f.Key.Proto == IPPROTO_ICMP || f.Key.Proto == IPPROTO_ICMPV6 {
		return f.GenStart == atomic.LoadUint64(&lostGen)
	}
	return false
}

func printOpen(f *Flow) {
	fmt.Printf("OPEN  %-5s pid=%d(%s) cookie=%d  %s -> %s%s%s\n",
		protoStr(f.Key.Proto),
		f.Key.TGID, f.Comm, f.Key.Cookie,
		fmtEndpoint(f.Key.Family, f.Local, f.LocalScope, f.Lport, f.Key.Proto),
		fmtEndpoint(f.Key.Family, f.Remote, f.RemoteScope, f.Rport, f.Key.Proto),
		incompleteNote(f),
		maybeLostNote(f),
	)
}

func printClose(f *Flow, reason string) {
	age := time.Since(f.FirstSeen).Truncate(time.Millisecond)
	fmt.Printf("CLOSE %-5s pid=%d(%s) cookie=%d  %s -> %s  out=%dB/%dp in=%dB/%dp  age=%s reason=%s%s%s\n",
		protoStr(f.Key.Proto),
		f.Key.TGID, f.Comm, f.Key.Cookie,
		fmtEndpoint(f.Key.Family, f.Local, f.LocalScope, f.Lport, f.Key.Proto),
		fmtEndpoint(f.Key.Family, f.Remote, f.RemoteScope, f.Rport, f.Key.Proto),
		f.OutBytes, f.OutPkts, f.InBytes, f.InPkts,
		age, reason,
		incompleteNote(f),
		maybeLostNote(f),
	)
}

func main() {
	flag.Parse()
	log.SetFlags(log.LstdFlags | log.Lmicroseconds)

	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("failed to remove memlock: %v", err)
	}
	if err := loadBpfObjects(&objs, nil); err != nil {
		log.Fatalf("failed to load bpf objects: %v", err)
	}
	defer objs.Close()

	if *flgPprof {
		go func() {
			log.Printf("pprof on %s", *flgPprofAddr)
			_ = http.ListenAndServe(*flgPprofAddr, nil)
		}()
	}

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

	attach("syscalls", "sys_enter_bind", objs.TraceBindEnter)
	attach("syscalls", "sys_exit_bind", objs.TraceBindExit)

	attach("syscalls", "sys_enter_connect", objs.TraceConnectEnter)
	attach("syscalls", "sys_exit_connect", objs.TraceConnectExit)

	attach("syscalls", "sys_enter_accept4", objs.TraceAccept4Enter)
	attach("syscalls", "sys_exit_accept4", objs.TraceAccept4Exit)
	attach("syscalls", "sys_enter_accept", objs.TraceAcceptEnter)
	attach("syscalls", "sys_exit_accept", objs.TraceAcceptExit)

	attach("syscalls", "sys_enter_close", objs.TraceCloseEnter)

	attach("syscalls", "sys_enter_sendto", objs.TraceSendtoEnter)
	attach("syscalls", "sys_exit_sendto", objs.TraceSendtoExit)
	attach("syscalls", "sys_enter_recvfrom", objs.TraceRecvfromEnter)
	attach("syscalls", "sys_exit_recvfrom", objs.TraceRecvfromExit)

	attach("syscalls", "sys_enter_sendmsg", objs.TraceSendmsgEnter)
	attach("syscalls", "sys_exit_sendmsg", objs.TraceSendmsgExit)
	attach("syscalls", "sys_enter_recvmsg", objs.TraceRecvmsgEnter)
	attach("syscalls", "sys_exit_recvmsg", objs.TraceRecvmsgExit)

	attach("syscalls", "sys_enter_sendmmsg", objs.TraceSendmmsgEnter)
	attach("syscalls", "sys_exit_sendmmsg", objs.TraceSendmmsgExit)
	attach("syscalls", "sys_enter_recvmmsg", objs.TraceRecvmmsgEnter)
	attach("syscalls", "sys_exit_recvmmsg", objs.TraceRecvmmsgExit)

	attach("syscalls", "sys_enter_write", objs.TraceWriteEnter)
	attach("syscalls", "sys_exit_write", objs.TraceWriteExit)
	attach("syscalls", "sys_enter_read", objs.TraceReadEnter)
	attach("syscalls", "sys_exit_read", objs.TraceReadExit)

	perfBytes := *flgPerfMB * 1024 * 1024
	if perfBytes < 256*1024 {
		perfBytes = 256 * 1024
	}
	rd, err := perf.NewReader(objs.TraceEvents, perfBytes)
	if err != nil {
		log.Fatalf("perf.NewReader: %v", err)
	}
	defer rd.Close()

	stop := make(chan os.Signal, 1)
	signal.Notify(stop, os.Interrupt, syscall.SIGTERM)

	type evWrap struct {
		ev  bpfTraceInfo
		now time.Time
	}

	evCh := make(chan evWrap, 16384)

	go func() {
		defer close(evCh)
		for {
			rec, e := rd.Read()
			if e != nil {
				if errors.Is(e, perf.ErrClosed) {
					return
				}
				continue
			}
			if rec.LostSamples != 0 {
				total := atomic.AddUint64(&lostTotal, rec.LostSamples)
				gen := atomic.AddUint64(&lostGen, 1)
				log.Printf("PERF_LOST chunk=%d total=%d gen=%d", rec.LostSamples, total, gen)
			}
			if len(rec.RawSample) < int(unsafe.Sizeof(bpfTraceInfo{})) {
				continue
			}
			ev := *(*bpfTraceInfo)(unsafe.Pointer(&rec.RawSample[0]))
			evCh <- evWrap{ev: ev, now: time.Now()}
		}
	}()

	flows := make(map[FlowKey]*Flow, 8192)
	ticker := time.NewTicker(*flgSweep)
	defer ticker.Stop()

	log.Println("OPEN/CLOSE only (TCP/UDP/ICMP) + PERF_LOST generation. Press Ctrl+C to exit")

	shouldKeep := func(pid uint32, comm string) bool {
		if comm == "" || comm == selfName {
			return false
		}
		if *flgOnlyPID != 0 && int(pid) != *flgOnlyPID {
			return false
		}
		if *flgOnlyComm != "" && !strings.Contains(comm, *flgOnlyComm) {
			return false
		}
		return true
	}

	upgradeKeyIfNeeded := func(key FlowKey) (FlowKey, *Flow) {
		if key.PeerMode != 1 {
			return key, nil
		}
		if key.Proto != IPPROTO_UDP && key.Proto != IPPROTO_ICMP && key.Proto != IPPROTO_ICMPV6 {
			return key, nil
		}
		base := key
		base.PeerMode = 0
		base.Rport = 0
		base.RemoteScope = 0
		for i := range base.Remote {
			base.Remote[i] = 0
		}
		if fb := flows[base]; fb != nil {
			delete(flows, base)
			fb.Key = key
			flows[key] = fb
			return key, fb
		}
		return key, nil
	}

	closeByCookie := func(tgid uint32, cookie uint64, reason string) {
		for k, f := range flows {
			if k.TGID == tgid && k.Cookie == cookie {
				if dropZeroFlow(f) {
					delete(flows, k)
					continue
				}
				if !f.OpenedPrinted && flowReadyToOpen(f) {
					printOpen(f)
					f.OpenedPrinted = true
				}
				if f.OpenedPrinted {
					printClose(f, reason)
				}
				delete(flows, k)
			}
		}
	}

	lastLost := uint64(0)
	lastTick := time.Now()

	for {
		select {
		case <-stop:
			_ = rd.Close()
			log.Printf("PERF_LOST_TOTAL total=%d gen=%d", atomic.LoadUint64(&lostTotal), atomic.LoadUint64(&lostGen))
			for _, f := range flows {
				if dropZeroFlow(f) {
					continue
				}
				if f.OpenedPrinted {
					printClose(f, "signal")
				}
			}
			log.Println("Exiting...")
			return

		case <-ticker.C:
			now := time.Now()

			total := atomic.LoadUint64(&lostTotal)
			delta := total - lastLost
			dt := now.Sub(lastTick)
			if delta > 0 {
				log.Printf("PERF_LOST_RATE lost=%d in=%s total=%d gen=%d evCh=%d/%d flows=%d",
					delta, dt.Truncate(time.Millisecond),
					total, atomic.LoadUint64(&lostGen),
					len(evCh), cap(evCh), len(flows),
				)
			}
			lastLost = total
			lastTick = now

			for k, f := range flows {
				if now.Sub(f.LastSeen) > *flgTTL {
					if dropZeroFlow(f) {
						delete(flows, k)
						continue
					}
					if !f.OpenedPrinted && flowReadyToOpen(f) {
						printOpen(f)
						f.OpenedPrinted = true
					}
					if f.OpenedPrinted {
						printClose(f, "idle")
					}
					delete(flows, k)
				}
			}

		case w, ok := <-evCh:
			if !ok {
				return
			}

			ev := w.ev
			comm := commString(ev.Comm)
			if !shouldKeep(ev.Tgid, comm) {
				continue
			}

			evt := uint8(ev.Event)
			proto := uint8(ev.Proto)
			family := uint16(ev.Family)

			if !protoAllowed(proto) {
				continue
			}
			if family != AF_INET && family != AF_INET6 {
				continue
			}

			if evt == EV_CLOSE {
				closeByCookie(ev.Tgid, ev.Cookie, "close()")
				continue
			}

			key := makeKey(ev)
			key, upgraded := upgradeKeyIfNeeded(key)

			f := flows[key]
			if f == nil {
				if upgraded != nil {
					f = upgraded
				} else {
					f = &Flow{
						Key:       key,
						Comm:      comm,
						FirstSeen: w.now,
						LastSeen:  w.now,
						GenStart:  atomic.LoadUint64(&lostGen),
					}
					flows[key] = f
				}
			}

			f.LastSeen = w.now
			applyEndpoints(f, ev)

			switch evt {
			case EV_SENDMMSG:
				if ev.Ret > 0 {
					f.OutBytes += uint64(ev.Ret)
				}
				if ev.State > 0 {
					f.OutPkts += uint64(ev.State)
				} else {
					f.OutPkts++
				}

			case EV_RECVMMSG:
				if ev.Ret > 0 {
					f.InBytes += uint64(ev.Ret)
				}
				if ev.State > 0 {
					f.InPkts += uint64(ev.State)
				} else {
					f.InPkts++
				}

			default:
				if isSend(evt) && ev.Ret > 0 {
					f.OutBytes += uint64(ev.Ret)
					f.OutPkts++
				} else if isRecv(evt) && ev.Ret > 0 {
					f.InBytes += uint64(ev.Ret)
					f.InPkts++
				}
			}

			if !f.OpenedPrinted && flowReadyToOpen(f) {
				printOpen(f)
				f.OpenedPrinted = true
			}
		}
	}
}
