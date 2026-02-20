package main

import (
	"bytes"
	"encoding/binary"
	"errors"
	"flag"
	"fmt"
	"log"
	"net/http"
	_ "net/http/pprof"
	"os"
	"os/signal"
	"path/filepath"
	"syscall"
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
)

var (
	flgPerfMB    = flag.Int("perfMB", 4, "perf buffer size in MB")
	flgDebug     = flag.Bool("debug", false, "debug logs")
	flgPprof     = flag.Bool("pprof", true, "enable pprof")
	flgPprofAddr = flag.String("pprofAddr", ":6060", "pprof listen addr")
	flgSample    = flag.Uint64("sample", 1, "print every Nth event")
)

func dbg(format string, args ...any) {
	if *flgDebug {
		log.Printf("[DBG] "+format, args...)
	}
}

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

func protoStr(p uint32) string {
	switch uint8(p) {
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

func evName(code uint8) string {
	switch code {
	case EV_SENDTO:
		return "SENDTO"
	case EV_RECVFROM:
		return "RECVFROM"
	case EV_SENDMSG:
		return "SENDMSG"
	case EV_RECVMSG:
		return "RECVMSG"
	case EV_CONNECT:
		return "CONNECT"
	case EV_ACCEPT:
		return "ACCEPT"
	case EV_BINDOK:
		return "BIND"
	default:
		return fmt.Sprintf("EV%d", code)
	}
}

// IPv4 u32 from kernel is network-order but looks swapped on little-endian.
// This is the same trick you already used.
func ip4BytesFromU32Net(x uint32) (b [4]byte) {
	binary.LittleEndian.PutUint32(b[:], x)
	return
}

func fmtIPv4(u32 uint32) (string, [16]byte) {
	var key [16]byte
	b := ip4BytesFromU32Net(u32)
	copy(key[:4], b[:])
	return fmt.Sprintf("%d.%d.%d.%d", b[0], b[1], b[2], b[3]), key
}

func fmtIPv6(a [16]uint8) (string, [16]byte) {
	var key [16]byte
	copy(key[:], a[:])
	// simple (no compression) like your output
	s := fmt.Sprintf("%x:%x:%x:%x:%x:%x:%x:%x",
		uint16(a[0])<<8|uint16(a[1]),
		uint16(a[2])<<8|uint16(a[3]),
		uint16(a[4])<<8|uint16(a[5]),
		uint16(a[6])<<8|uint16(a[7]),
		uint16(a[8])<<8|uint16(a[9]),
		uint16(a[10])<<8|uint16(a[11]),
		uint16(a[12])<<8|uint16(a[13]),
		uint16(a[14])<<8|uint16(a[15]),
	)
	return s, key
}

func isAllZero16(b [16]byte) bool {
	for i := 0; i < 16; i++ {
		if b[i] != 0 {
			return false
		}
	}
	return true
}

func fmtEndp(family uint16, ipStr string, ipKey [16]byte, port uint16, forICMP bool) string {
	if forICMP {
		if isAllZero16(ipKey) {
			return "*"
		}
		return ipStr
	}

	if isAllZero16(ipKey) {
		return fmt.Sprintf("*:%d", port)
	}
	if family == AF_INET6 {
		return fmt.Sprintf("[%s]:%d", ipStr, port)
	}
	return fmt.Sprintf("%s:%d", ipStr, port)
}

func main() {
	flag.Parse()
	log.SetFlags(log.LstdFlags | log.Lmicroseconds)

	if *flgSample < 1 {
		*flgSample = 1
	}

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

	// attach tracepoints
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

	log.Println("Press Ctrl+C to exit")

	var printed uint64

	for {
		select {
		case <-stop:
			log.Println("Exiting...")
			return
		default:
		}

		rec, err := rd.Read()
		if err != nil {
			if errors.Is(err, perf.ErrClosed) {
				return
			}
			continue
		}
		if rec.LostSamples != 0 {
			dbg("lost samples: %d", rec.LostSamples)
			continue
		}
		if len(rec.RawSample) < int(unsafe.Sizeof(bpfTraceInfo{})) {
			continue
		}

		ev := *(*bpfTraceInfo)(unsafe.Pointer(&rec.RawSample[0]))
		comm := commString(ev.Comm)
		if comm == "" || comm == selfName {
			continue
		}

		printed++
		if printed%*flgSample != 0 {
			continue
		}

		family := uint16(ev.Family)
		sport := uint16(ev.Sport)
		dport := uint16(ev.Dport)

		proto := ev.Proto
		pr := protoStr(proto)
		evn := evName(ev.Sysexit)

		tgid := ev.Tgid
		tid := ev.Tid
		fd := ev.Fd
		ret := ev.Ret
		cookie := ev.Cookie

		var srcIPStr, dstIPStr string
		var srcKey, dstKey [16]byte

		if family == AF_INET {
			srcIPStr, srcKey = fmtIPv4(ev.SrcIP.S_addr)
			dstIPStr, dstKey = fmtIPv4(ev.DstIP.S_addr)
		} else if family == AF_INET6 {
			srcIPStr, srcKey = fmtIPv6(ev.SrcIP6.In6U.U6Addr8)
			dstIPStr, dstKey = fmtIPv6(ev.DstIP6.In6U.U6Addr8)
		} else {
			continue
		}

		isICMP := uint8(proto) == IPPROTO_ICMP || uint8(proto) == IPPROTO_ICMPV6

		srcEp := fmtEndp(family, srcIPStr, srcKey, sport, isICMP)
		dstEp := fmtEndp(family, dstIPStr, dstKey, dport, isICMP)

		// output close to your style + add tid+cookie
		if isICMP {
			fmt.Printf("%s %-7s pid=%d(%s) tid=%d fd=%d cookie=%d ret=%d  %s -> %s\n",
				pr, evn, tgid, comm, tid, fd, cookie, ret, srcEp, dstEp)
			continue
		}

		fmt.Printf("%s %-7s pid=%d(%s) tid=%d fd=%d cookie=%d ret=%d  %s -> %s\n",
			pr, evn, tgid, comm, tid, fd, cookie, ret, srcEp, dstEp)
	}
}
