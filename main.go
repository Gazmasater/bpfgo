package main

import (
	"bytes"
	"encoding/binary"
	"errors"
	"flag"
	"fmt"
	"log"
	"os"
	"os/signal"
	"path/filepath"
	"runtime"
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

//go:generate go run github.com/cilium/ebpf/cmd/bpf2go -target amd64 -type trace_info -type tls_chunk_t bpf trace.c -- -I.

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
	EV_CLOSE    = 30

	EV_SKB_OUT = 40

	// must match trace.c
	EV_TLS_CHUNK = 50
)

var (
	flgPerfMB = flag.Int("perfMB", 8, "perf ring total budget in MB (per reader)")
	flgTTL    = flag.Duration("ttl", 5*time.Second, "idle TTL for flow close")
	flgSweep  = flag.Duration("sweep", 1*time.Second, "sweep interval")

	flgOnlyPID  = flag.Int("pid", 0, "only this pid (0=all)")
	flgOnlyComm = flag.String("comm", "", "only comm containing substring (empty=all)")

	flgMmsg = flag.Bool("mmsg", true, "trace sendmmsg/recvmmsg")
	flgRW   = flag.Bool("rw", true, "trace read/write on socket fds")

	flgSNI     = flag.Bool("sni", true, "parse SNI from TLS ClientHello")
	flgSniTTL  = flag.Duration("sniTTL", 30*time.Second, "SNI cache TTL")
	flgTlsKeep = flag.Duration("tlsKeep", 2*time.Second, "keep partial TLS buffers for this long")
)

/* --------------------- small helpers --------------------- */

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

// kernel u32 IPv4 is network-order but appears swapped on little-endian
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

func addrStr(family uint16, ip [16]byte) string {
	if isAllZero16(ip) {
		return "*"
	}
	if family == AF_INET6 {
		return fmtIPv6Full(ip)
	}
	return fmtIPv4FromKey(ip)
}

func fmtEndpoint(family uint16, ip [16]byte, port uint16, proto uint8) string {
	if proto == IPPROTO_ICMP || proto == IPPROTO_ICMPV6 {
		return addrStr(family, ip)
	}
	if family == AF_INET6 && !isAllZero16(ip) {
		return fmt.Sprintf("[%s]:%d", addrStr(family, ip), port)
	}
	return fmt.Sprintf("%s:%d", addrStr(family, ip), port)
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

/* --------------------- TLS SNI parsing --------------------- */

func u16(b []byte, i int) (uint16, bool) {
	if i+2 > len(b) {
		return 0, false
	}
	return uint16(b[i])<<8 | uint16(b[i+1]), true
}

// expects one TLS record starting at buf[0] (type 0x16)
func parseSNIFromTLSRecord(buf []byte) string {
	if len(buf) < 5 {
		return ""
	}
	if buf[0] != 0x16 { // Handshake
		return ""
	}
	if buf[1] != 0x03 {
		return ""
	}
	recLenU, ok := u16(buf, 3)
	if !ok {
		return ""
	}
	total := int(recLenU) + 5
	if total < 6 || len(buf) < total {
		return ""
	}

	p := 5
	if p+4 > total {
		return ""
	}
	if buf[p] != 0x01 { // ClientHello
		return ""
	}
	hsLen := int(buf[p+1])<<16 | int(buf[p+2])<<8 | int(buf[p+3])
	p += 4
	if p+hsLen > total {
		return ""
	}

	// version(2)+random(32)
	if p+34 > total {
		return ""
	}
	p += 34

	// session id
	if p+1 > total {
		return ""
	}
	sidLen := int(buf[p])
	p++
	if p+sidLen > total {
		return ""
	}
	p += sidLen

	// cipher suites
	csLenU, ok := u16(buf, p)
	if !ok {
		return ""
	}
	csLen := int(csLenU)
	p += 2
	if p+csLen > total {
		return ""
	}
	p += csLen

	// compression
	if p+1 > total {
		return ""
	}
	cmLen := int(buf[p])
	p++
	if p+cmLen > total {
		return ""
	}
	p += cmLen

	// extensions
	extLenU, ok := u16(buf, p)
	if !ok {
		return ""
	}
	extLen := int(extLenU)
	p += 2
	if p+extLen > total {
		return ""
	}
	extEnd := p + extLen

	for p+4 <= extEnd {
		typU, _ := u16(buf, p)
		lU, _ := u16(buf, p+2)
		typ := int(typU)
		l := int(lU)
		p += 4
		if p+l > extEnd {
			return ""
		}

		if typ == 0x0000 { // server_name
			if p+2 > extEnd {
				return ""
			}
			listLenU, _ := u16(buf, p)
			listLen := int(listLenU)
			q := p + 2
			end := q + listLen
			if end > p+l {
				return ""
			}
			for q+3 <= end {
				nameType := buf[q]
				q++
				nameLenU, _ := u16(buf, q)
				nameLen := int(nameLenU)
				q += 2
				if q+nameLen > end {
					return ""
				}
				if nameType == 0 {
					host := strings.TrimSpace(string(buf[q : q+nameLen]))
					if host != "" {
						return host
					}
				}
				q += nameLen
			}
			return ""
		}

		p += l
	}

	return ""
}

/* --------------------- TLS reassembly (fix for "rare SNI") --------------------- */

type tlsReasm struct {
	buf  []byte
	seen time.Time
	done bool // SNI already found for this cookie
}

const (
	tlsMaxBuf = 16 * 1024 // cap memory per cookie
)

// find TLS handshake record header in b (16 03 xx len len)
func findTLSHeader(b []byte) int {
	// naive scan is fast enough for <=16KB
	for i := 0; i+5 <= len(b); i++ {
		if b[i] == 0x16 && b[i+1] == 0x03 {
			v := b[i+2]
			if v >= 0x01 && v <= 0x04 { // TLS1.0..1.3 record version bytes
				return i
			}
		}
	}
	return -1
}

// Feed new chunk, try to extract complete TLS records and parse SNI.
// Returns (sni, ok).
func (t *tlsReasm) Feed(now time.Time, chunk []byte) (string, bool) {
	if t.done {
		return "", false
	}
	t.seen = now

	// append with cap
	if len(chunk) > 0 {
		need := len(t.buf) + len(chunk)
		if need > tlsMaxBuf {
			// keep tail (most recent) to avoid endless growth
			drop := need - tlsMaxBuf
			if drop >= len(t.buf) {
				t.buf = t.buf[:0]
			} else {
				t.buf = append([]byte(nil), t.buf[drop:]...)
			}
		}
		t.buf = append(t.buf, chunk...)
	}

	// reassembly loop: align to header, wait full record, parse, consume
	for {
		if len(t.buf) < 5 {
			return "", false
		}

		hi := findTLSHeader(t.buf)
		if hi < 0 {
			// keep last 4 bytes just in case header crosses boundary
			if len(t.buf) > 4 {
				t.buf = append([]byte(nil), t.buf[len(t.buf)-4:]...)
			}
			return "", false
		}
		if hi > 0 {
			// drop garbage before header
			t.buf = t.buf[hi:]
			if len(t.buf) < 5 {
				return "", false
			}
		}

		recLen := int(t.buf[3])<<8 | int(t.buf[4])
		total := 5 + recLen
		if recLen <= 0 || total > tlsMaxBuf {
			// invalid length -> drop 1 byte and resync
			t.buf = t.buf[1:]
			continue
		}
		if len(t.buf) < total {
			// wait more
			return "", false
		}

		rec := t.buf[:total]
		if sni := parseSNIFromTLSRecord(rec); sni != "" {
			t.done = true
			return sni, true
		}

		// consume this record and continue scanning next records
		t.buf = t.buf[total:]
	}
}

type sniEntry struct {
	Name string
	Seen time.Time
}

/* --------------------- Flow model --------------------- */

type FlowKey struct {
	TGID   uint32
	Cookie uint64
	Proto  uint8
	Family uint16
}

type Flow struct {
	Key  FlowKey
	Comm string

	Local  [16]byte
	Lport  uint16
	Remote [16]byte
	Rport  uint16

	FirstSeen time.Time
	LastSeen  time.Time

	InBytes  uint64
	OutBytes uint64
	InPkts   uint64
	OutPkts  uint64

	OpenedPrinted bool
	GenStart      uint64

	TLSSNI string
}

func makeKey(ev bpfTraceInfo) FlowKey {
	return FlowKey{
		TGID:   ev.Tgid,
		Cookie: ev.Cookie,
		Proto:  uint8(ev.Proto),
		Family: uint16(ev.Family),
	}
}

func applyEndpoints(f *Flow, ev bpfTraceInfo) {
	evt := uint8(ev.Event)

	switch {
	case isSend(evt) || evt == EV_CONNECT:
		if !isAllZero16(srcKeyFromEvent(ev)) {
			f.Local = srcKeyFromEvent(ev)
		}
		if !isAllZero16(dstKeyFromEvent(ev)) {
			f.Remote = dstKeyFromEvent(ev)
		}
		if ev.Sport != 0 {
			f.Lport = uint16(ev.Sport)
		}
		if ev.Dport != 0 {
			f.Rport = uint16(ev.Dport)
		}

	case isRecv(evt):
		if !isAllZero16(dstKeyFromEvent(ev)) {
			f.Local = dstKeyFromEvent(ev)
		}
		if !isAllZero16(srcKeyFromEvent(ev)) {
			f.Remote = srcKeyFromEvent(ev)
		}
		if ev.Dport != 0 {
			f.Lport = uint16(ev.Dport)
		}
		if ev.Sport != 0 {
			f.Rport = uint16(ev.Sport)
		}

	case evt == EV_BINDOK:
		if !isAllZero16(srcKeyFromEvent(ev)) {
			f.Local = srcKeyFromEvent(ev)
		}
		if ev.Sport != 0 {
			f.Lport = uint16(ev.Sport)
		}

	case evt == EV_ACCEPT:
		if !isAllZero16(dstKeyFromEvent(ev)) {
			f.Local = dstKeyFromEvent(ev)
		}
		if !isAllZero16(srcKeyFromEvent(ev)) {
			f.Remote = srcKeyFromEvent(ev)
		}
		if ev.Dport != 0 {
			f.Lport = uint16(ev.Dport)
		}
		if ev.Sport != 0 {
			f.Rport = uint16(ev.Sport)
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

func printOpen(f *Flow) {
	sni := ""
	if f.TLSSNI != "" {
		sni = " sni=" + f.TLSSNI
	}
	fmt.Printf("OPEN  %-5s pid=%d(%s) cookie=%d  %s -> %s%s\n",
		protoStr(f.Key.Proto),
		f.Key.TGID, f.Comm, f.Key.Cookie,
		fmtEndpoint(f.Key.Family, f.Local, f.Lport, f.Key.Proto),
		fmtEndpoint(f.Key.Family, f.Remote, f.Rport, f.Key.Proto),
		sni,
	)
}

func printClose(f *Flow, reason string) {
	sni := ""
	if f.TLSSNI != "" {
		sni = " sni=" + f.TLSSNI
	}
	age := time.Since(f.FirstSeen).Truncate(time.Millisecond)
	fmt.Printf("CLOSE %-5s pid=%d(%s) cookie=%d  %s -> %s  out=%dB/%dp in=%dB/%dp  age=%s reason=%s%s\n",
		protoStr(f.Key.Proto),
		f.Key.TGID, f.Comm, f.Key.Cookie,
		fmtEndpoint(f.Key.Family, f.Local, f.Lport, f.Key.Proto),
		fmtEndpoint(f.Key.Family, f.Remote, f.Rport, f.Key.Proto),
		f.OutBytes, f.OutPkts, f.InBytes, f.InPkts,
		age, reason,
		sni,
	)
}

/* --------------------- perf reader helpers --------------------- */

func openPerfReaderTotalBudget(m *ebpf.Map, totalMB int) (*perf.Reader, int, error) {
	totalBytes := totalMB * 1024 * 1024
	if totalBytes < 256*1024 {
		totalBytes = 256 * 1024
	}
	nCPU := runtime.NumCPU()
	perCPU := totalBytes / nCPU
	if perCPU < 256*1024 {
		perCPU = 256 * 1024
	}
	rd, err := perf.NewReader(m, perCPU)
	return rd, perCPU, err
}

/* --------------------- main --------------------- */

var lostTrace uint64
var lostTLS uint64
var lostGen uint64

func main() {
	flag.Parse()
	log.SetFlags(log.LstdFlags | log.Lmicroseconds)

	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("rlimit.RemoveMemlock: %v", err)
	}
	if err := loadBpfObjects(&objs, nil); err != nil {
		log.Fatalf("loadBpfObjects: %v", err)
	}
	defer objs.Close()

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

	// baseline
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

	if *flgMmsg {
		attach("syscalls", "sys_enter_sendmmsg", objs.TraceSendmmsgEnter)
		attach("syscalls", "sys_exit_sendmmsg", objs.TraceSendmmsgExit)
		attach("syscalls", "sys_enter_recvmmsg", objs.TraceRecvmmsgEnter)
		attach("syscalls", "sys_exit_recvmmsg", objs.TraceRecvmmsgExit)
	}
	if *flgRW {
		attach("syscalls", "sys_enter_write", objs.TraceWriteEnter)
		attach("syscalls", "sys_exit_write", objs.TraceWriteExit)
		attach("syscalls", "sys_enter_read", objs.TraceReadEnter)
		attach("syscalls", "sys_exit_read", objs.TraceReadExit)
	}

	// skb hint (можешь вернуть обработку позже)
	attach("net", "net_dev_queue", objs.TraceNetDevQueue)

	// perf readers
	traceRd, perCPU, err := openPerfReaderTotalBudget(objs.TraceEvents, *flgPerfMB)
	if err != nil {
		log.Fatalf("perf.NewReader trace: %v", err)
	}
	defer traceRd.Close()

	var tlsRd *perf.Reader
	if *flgSNI {
		tlsRd, _, err = openPerfReaderTotalBudget(objs.TlsEvents, *flgPerfMB)
		if err != nil {
			log.Fatalf("perf.NewReader tls: %v", err)
		}
		defer tlsRd.Close()
	}

	log.Printf("perf trace per-cpu=%dKB total~=%dMB cpus=%d", perCPU/1024, (perCPU*runtime.NumCPU())/(1024*1024), runtime.NumCPU())
	log.Printf("started. Ctrl+C to exit")

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
			rec, e := traceRd.Read()
			if e != nil {
				if errors.Is(e, perf.ErrClosed) {
					return
				}
				continue
			}
			if rec.LostSamples != 0 {
				atomic.AddUint64(&lostTrace, rec.LostSamples)
				atomic.AddUint64(&lostGen, 1)
			}
			if len(rec.RawSample) < int(unsafe.Sizeof(bpfTraceInfo{})) {
				continue
			}
			ev := *(*bpfTraceInfo)(unsafe.Pointer(&rec.RawSample[0]))
			evCh <- evWrap{ev: ev, now: time.Now()}
		}
	}()

	type tlsWrap struct {
		ev  bpfTlsChunkT
		now time.Time
	}
	tlsCh := make(chan tlsWrap, 8192)

	if *flgSNI {
		go func() {
			defer close(tlsCh)
			for {
				rec, e := tlsRd.Read()
				if e != nil {
					if errors.Is(e, perf.ErrClosed) {
						return
					}
					continue
				}
				if rec.LostSamples != 0 {
					atomic.AddUint64(&lostTLS, rec.LostSamples)
					atomic.AddUint64(&lostGen, 1)
				}
				if len(rec.RawSample) < int(unsafe.Sizeof(bpfTlsChunkT{})) {
					continue
				}
				ev := *(*bpfTlsChunkT)(unsafe.Pointer(&rec.RawSample[0]))
				tlsCh <- tlsWrap{ev: ev, now: time.Now()}
			}
		}()
	}

	flows := make(map[FlowKey]*Flow, 8192)

	// TLS + SNI state
	tlsByCookie := make(map[uint64]*tlsReasm, 8192)
	sniByCookie := make(map[uint64]sniEntry, 8192)

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

	attachSNIToFlow := func(f *Flow, now time.Time) {
		if f.TLSSNI != "" {
			return
		}
		if se, ok := sniByCookie[f.Key.Cookie]; ok && now.Sub(se.Seen) < *flgSniTTL {
			f.TLSSNI = se.Name
		}
	}

	closeByCookie := func(tgid uint32, cookie uint64, reason string) {
		for k, f := range flows {
			if k.TGID == tgid && k.Cookie == cookie {
				attachSNIToFlow(f, time.Now())
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

	ticker := time.NewTicker(*flgSweep)
	defer ticker.Stop()

	for {
		select {
		case <-stop:
			_ = traceRd.Close()
			if tlsRd != nil {
				_ = tlsRd.Close()
			}
			for _, f := range flows {
				attachSNIToFlow(f, time.Now())
				if !f.OpenedPrinted && flowReadyToOpen(f) {
					printOpen(f)
					f.OpenedPrinted = true
				}
				if f.OpenedPrinted {
					printClose(f, "signal")
				}
			}
			log.Printf("lost trace=%d tls=%d gen=%d", atomic.LoadUint64(&lostTrace), atomic.LoadUint64(&lostTLS), atomic.LoadUint64(&lostGen))
			return

		case tw, ok := <-tlsCh:
			if !ok {
				tlsCh = nil
				continue
			}
			ev := tw.ev
			if ev.Cookie == 0 || uint8(ev.Event) != EV_TLS_CHUNK {
				continue
			}

			ln := int(ev.Len)
			if ln <= 0 {
				continue
			}
			if ln > len(ev.Data) {
				ln = len(ev.Data)
			}
			chunk := ev.Data[:ln]

			tr := tlsByCookie[ev.Cookie]
			if tr == nil {
				tr = &tlsReasm{buf: make([]byte, 0, 2048)}
				tlsByCookie[ev.Cookie] = tr
			}

			if sni, ok := tr.Feed(tw.now, chunk); ok {
				sniByCookie[ev.Cookie] = sniEntry{Name: sni, Seen: tw.now}
				delete(tlsByCookie, ev.Cookie)

				log.Printf("SNI cookie=%d %s", ev.Cookie, sni)

				// update existing flows
				for _, f := range flows {
					if f.Key.Cookie == ev.Cookie && f.TLSSNI == "" {
						f.TLSSNI = sni
					}
				}
			}

		case w, ok := <-evCh:
			if !ok {
				return
			}
			ev := w.ev
			evt := uint8(ev.Event)
			proto := uint8(ev.Proto)
			family := uint16(ev.Family)

			if evt == EV_SKB_OUT {
				continue
			}

			comm := commString(ev.Comm)
			if !shouldKeep(ev.Tgid, comm) {
				continue
			}
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
			f := flows[key]
			if f == nil {
				f = &Flow{
					Key:       key,
					Comm:      comm,
					FirstSeen: w.now,
					LastSeen:  w.now,
					GenStart:  atomic.LoadUint64(&lostGen),
				}
				flows[key] = f

				// attach SNI if already known
				attachSNIToFlow(f, w.now)
			}

			f.LastSeen = w.now
			applyEndpoints(f, ev)

			// accounting
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

		case <-ticker.C:
			now := time.Now()

			// sweep flows by TTL
			for k, f := range flows {
				if now.Sub(f.LastSeen) > *flgTTL {
					attachSNIToFlow(f, now)
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

			// sweep partial TLS buffers (handshake is short)
			for c, tr := range tlsByCookie {
				if now.Sub(tr.seen) > *flgTlsKeep {
					delete(tlsByCookie, c)
				}
			}

			// sweep sni cache
			for c, se := range sniByCookie {
				if now.Sub(se.Seen) > *flgSniTTL {
					delete(sniByCookie, c)
				}
			}

			// periodic loss note (optional)
			lt := atomic.LoadUint64(&lostTrace)
			ll := atomic.LoadUint64(&lostTLS)
			if lt != 0 || ll != 0 {
				// можно закомментировать если шумит
				// log.Printf("lost trace=%d tls=%d gen=%d", lt, ll, atomic.LoadUint64(&lostGen))
			}
		}
	}
}
