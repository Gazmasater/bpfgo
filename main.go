package main

import (
	"errors"
	"flag"
	"log"
	"net/http"
	_ "net/http/pprof"
	"os"
	"os/signal"
	"path/filepath"
	"runtime"
	"strings"
	"sync/atomic"
	"syscall"
	"time"
	"unsafe"

	"github.com/cilium/ebpf/perf"
	"github.com/cilium/ebpf/rlimit"
)

//go:generate go run github.com/cilium/ebpf/cmd/bpf2go -tags linux -target amd64 -type trace_info -type tls_chunk_event bpf internal/ebpf/trace.bpf.c -- -Iinternal/ebpf

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

	// skb-derived L3 hint (real selected src IP)
	EV_SKB_OUT   = 40
	EV_TLS_CHUNK = 100
)

var (
	flgPerfMB    = flag.Int("perfMB", 8, "perf buffer total budget in MB (divided per-CPU with fallback)")
	flgPprof     = flag.Bool("pprof", false, "enable pprof")
	flgPprofAddr = flag.String("pprofAddr", "127.0.0.1:6060", "pprof listen addr")

	flgTTL   = flag.Duration("ttl", 5*time.Second, "idle TTL for flow close")
	flgSweep = flag.Duration("print", 1*time.Second, "TTL sweep interval + perf-loss rate logging interval")

	flgOnlyPID  = flag.Int("pid", 0, "only this pid (0=all)")
	flgOnlyComm = flag.String("comm", "", "only comm containing substring (empty=all)")

	flgRW   = flag.Bool("rw", true, "trace read/write on socket fds")
	flgMmsg = flag.Bool("mmsg", true, "trace sendmmsg/recvmmsg")

	// PTR resolve
	flgResolve        = flag.Bool("resolve", true, "reverse DNS resolve IPs (PTR) async")
	flgResolveTTL     = flag.Duration("resolveTTL", 30*time.Minute, "PTR cache TTL (positive)")
	flgResolveNegTTL  = flag.Duration("resolveNegTTL", 5*time.Minute, "PTR negative TTL (NXDOMAIN only)")
	flgResolveWorkers = flag.Int("resolveWorkers", 4, "PTR workers")
	flgResolveTimeout = flag.Duration("resolveTimeout", 2*time.Second, "PTR timeout")
	flgResolveQ       = flag.Int("resolveQ", 4096, "PTR queue size")
	flgHostState      = flag.Bool("hostState", true, "show alias state: pending/miss/no-ptr/skip")

	// known names from /etc/hosts
	flgHostsPrefill = flag.Bool("hostsPrefill", true, "prefill names from /etc/hosts")
	flgHostsFile    = flag.String("hostsFile", "/etc/hosts", "hosts file")
	flgHostsTTL     = flag.Duration("hostsTTL", 24*time.Hour, "hosts prefill TTL")

	// dns cache sweep
	flgResolveSweepEach = flag.Int("resolveSweepEach", 500, "dns cache sweep per tick")
	flgResolvePokeEach  = flag.Int("resolvePokeEach", 256, "poke flows per tick to resolve alias")

	// L3-hint cache ttl + OPEN delay to wait for skb-hint (to avoid *(any))
	flgL3          = flag.Bool("l3", true, "trace net_dev_queue L3 socket hints")
	flgL3TTL       = flag.Duration("l3ttl", 10*time.Second, "TTL for skb-derived L3 hints (cookie->src/dst)")
	flgOpenDelay   = flag.Duration("openDelay", 200*time.Millisecond, "delay OPEN print (max) to wait for skb hint to fill src ip")
	flgL3SweepEach = flag.Int("l3SweepEach", 500, "l3 hint sweep per tick")

	// SNI
	flgSNI        = flag.Bool("sni", true, "parse TLS ClientHello SNI from tls chunk events")
	flgSNITTL     = flag.Duration("sniTTL", 10*time.Minute, "TTL for cookie->SNI cache")
	flgSNIMaxKeep = flag.Int("sniMax", 200000, "max cookie->SNI entries in memory (best effort)")
)

/* ===== main ===== */

func main() {
	if p, err := loadDotEnvAuto(); err != nil {
		log.Printf("dotenv: %v", err)
	} else if p != "" {
		log.Printf("dotenv loaded: %s", p)
	}

	applyEnvToFlags()
	flag.Parse()

	log.SetFlags(log.LstdFlags | log.Lmicroseconds)

	if *flgResolve {
		dnsr = newDNSResolver(*flgResolveQ, *flgResolveWorkers, *flgResolveTTL, *flgResolveNegTTL, *flgResolveTimeout)
		defer dnsr.Close()
		if *flgHostsPrefill {
			parseHostsPrefill(*flgHostsFile, *flgHostsTTL, dnsr)
		}
	}

	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("failed to remove memlock: %v", err)
	}
	if err := loadBpfObjects(&objs, nil); err != nil {
		log.Fatalf("failed to load bpf objects: %v", err)
	}
	defer objs.Close()
	if err := setTLSCaptureEnabled(&objs, *flgSNI); err != nil {
		log.Fatalf("configure TLS capture: %v", err)
	}

	if *flgPprof {
		go func() {
			log.Printf("pprof on %s", *flgPprofAddr)
			if err := http.ListenAndServe(*flgPprofAddr, nil); err != nil && !errors.Is(err, http.ErrServerClosed) {
				log.Printf("pprof server: %v", err)
			}
		}()
	}

	selfName := filepath.Base(os.Args[0])

	links, err := attachProbeGroups(buildProbeGroups(&objs, *flgMmsg, *flgRW, *flgL3))
	if err != nil {
		log.Fatalf("attach probes: %v", err)
	}
	defer closeLinks(links)

	// main flow perf reader
	rd, perCPUBytes, err := openPerfReaderTotalBudget(objs.TraceEvents, *flgPerfMB)
	if err != nil {
		log.Fatalf("perf.NewReader(trace_events): %v", err)
	}
	defer rd.Close()

	log.Printf("perf ring trace_events per-cpu=%dKB total~=%dMB cpus=%d",
		perCPUBytes/1024, (perCPUBytes*runtime.NumCPU())/(1024*1024), runtime.NumCPU())

	// --- TLS reader (SNI) ---
	var tlsRD *perf.Reader
	var tlsAssembler *tlsAssembler
	sniC := newSNICache(*flgSNITTL, *flgSNIMaxKeep)

	if *flgSNI {
		if objs.TlsEvents == nil {
			log.Printf("SNI disabled: tls_events map is unavailable")
		} else {
			tlsRD, _, err = openPerfReaderTotalBudget(objs.TlsEvents, max(1, *flgPerfMB/2))
			if err != nil {
				log.Printf("SNI reader disabled: %v", err)
				tlsRD = nil
			}
		}
		tlsAssembler = newTLSAssembler(30*time.Second, *flgSNIMaxKeep)
	}

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

	// tlsCh (only if tlsRD enabled)
	type tlsWrap struct {
		ev  bpfTlsChunkEvent
		now time.Time
	}
	tlsCh := make(chan tlsWrap, 8192)

	if tlsRD != nil {
		go func() {
			defer close(tlsCh)
			for {
				rec, e := tlsRD.Read()
				if e != nil {
					if errors.Is(e, perf.ErrClosed) {
						return
					}
					continue
				}
				if len(rec.RawSample) < int(unsafe.Sizeof(bpfTlsChunkEvent{})) {
					continue
				}
				ev := *(*bpfTlsChunkEvent)(unsafe.Pointer(&rec.RawSample[0]))
				tlsCh <- tlsWrap{ev: ev, now: time.Now()}
			}
		}()
	} else {
		close(tlsCh)
	}

	flows := make(map[FlowKey]*Flow, 8192)
	flowsByCookie := make(map[uint64]map[FlowKey]*Flow, 8192)
	l3ByCookie := make(map[uint64]l3Info, 8192)
	addFlow := func(key FlowKey, f *Flow) {
		flows[key] = f
		bucket := flowsByCookie[key.Cookie]
		if bucket == nil {
			bucket = make(map[FlowKey]*Flow, 1)
			flowsByCookie[key.Cookie] = bucket
		}
		bucket[key] = f
	}
	removeFlow := func(key FlowKey) {
		delete(flows, key)
		bucket := flowsByCookie[key.Cookie]
		if bucket == nil {
			return
		}
		delete(bucket, key)
		if len(bucket) == 0 {
			delete(flowsByCookie, key.Cookie)
		}
	}

	ticker := time.NewTicker(*flgSweep)
	defer ticker.Stop()

	log.Printf("flow summaries (TCP/UDP/ICMP) + PTR + skb-hint + SNI=%t. Ctrl+C to exit", tlsRD != nil)

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
		base.Rscope = 0
		for i := range base.Remote {
			base.Remote[i] = 0
		}
		if fb := flows[base]; fb != nil {
			removeFlow(base)
			fb.Key = key
			addFlow(key, fb)
			return key, fb
		}
		return key, nil
	}

	closeByCookie := func(tgid uint32, cookie uint64, reason string) {
		for k, f := range flowsByCookie[cookie] {
			if k.TGID == tgid {
				if h, ok := l3ByCookie[cookie]; ok {
					applyL3HintToFlow(f, h)
				}
				if s, ok := sniC.Get(cookie); ok && f.SNI == "" {
					f.SNI = s
				}

				if dropZeroFlow(f) {
					removeFlow(k)
					continue
				}
				if !f.OpenedPrinted && flowReadyToPrintOpen(f) {
					printLiveFlow(f)
					f.OpenedPrinted = true
				}
				if f.OpenedPrinted {
					printFlow(f, reason)
				}
				removeFlow(k)
			}
		}
	}

	lastLost := uint64(0)
	lastTick := time.Now()

	for {
		select {
		case <-stop:
			_ = rd.Close()
			if tlsRD != nil {
				_ = tlsRD.Close()
			}
			log.Printf("PERF_LOST_TOTAL total=%d gen=%d", atomic.LoadUint64(&lostTotal), atomic.LoadUint64(&lostGen))
			for _, f := range flows {
				if dropZeroFlow(f) {
					continue
				}
				if !f.OpenedPrinted && flowReadyToPrintOpen(f) {
					printLiveFlow(f)
					f.OpenedPrinted = true
				}
				if f.OpenedPrinted {
					printFlow(f, "signal")
				}
			}
			log.Println("Exiting...")
			return

		case tw, ok := <-tlsCh:
			if !ok {
				// tls reader disabled/closed
				// continue serving flow events
				tlsCh = nil
				continue
			}
			if !*flgSNI || tlsAssembler == nil {
				continue
			}

			ev := tw.ev
			if ev.Cookie == 0 || ev.Proto != IPPROTO_TCP {
				continue
			}
			// usually only dport=443 matters
			if ev.Dport != 443 && ev.Sport != 443 {
				continue
			}

			n := int(ev.Len)
			if n <= 0 {
				continue
			}
			if n > len(ev.Data) {
				n = len(ev.Data)
			}
			chunk := make([]byte, n)
			copy(chunk, ev.Data[:n])

			if s, ok := tlsAssembler.Push(ev.Cookie, chunk); ok && s != "" {
				sniC.Put(ev.Cookie, s)
				if err := markTLSCaptureDone(&objs, ev.Cookie); err != nil {
					log.Printf("stop TLS capture for cookie=%d: %v", ev.Cookie, err)
				}
				// update live flows
				for _, f := range flowsByCookie[ev.Cookie] {
					if f.SNI == "" {
						f.SNI = s
					}
				}
			}

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

			if dnsr != nil && *flgResolveSweepEach > 0 {
				dnsr.SweepExpired(*flgResolveSweepEach)
			}

			// sweep L3 hints
			if *flgL3SweepEach > 0 {
				n := 0
				for c, h := range l3ByCookie {
					if h.expired(now, *flgL3TTL) {
						delete(l3ByCookie, c)
						n++
						if n >= *flgL3SweepEach {
							break
						}
					}
				}
			}

			// sweep sni cache + tls assembler (best effort)
			if *flgSNI {
				sniC.Sweep(2000)
				if tlsAssembler != nil {
					tlsAssembler.Sweep(2000)
				}
			}

			// poke some flows for resolve + l3 fill + sni fill
			if *flgResolvePokeEach > 0 {
				n := 0
				for _, f := range flows {
					if h, ok := l3ByCookie[f.Key.Cookie]; ok {
						applyL3HintToFlow(f, h)
					}
					if s, ok := sniC.Get(f.Key.Cookie); ok && f.SNI == "" {
						f.SNI = s
					}
					_ = remoteAliasCached(f)
					n++
					if n >= *flgResolvePokeEach {
						break
					}
				}
			}

			// TTL sweep flows
			for k, f := range flows {
				if now.Sub(f.LastSeen) > *flgTTL {
					if h, ok := l3ByCookie[f.Key.Cookie]; ok {
						applyL3HintToFlow(f, h)
					}
					if s, ok := sniC.Get(f.Key.Cookie); ok && f.SNI == "" {
						f.SNI = s
					}
					if dropZeroFlow(f) {
						removeFlow(k)
						continue
					}
					if !f.OpenedPrinted && flowReadyToPrintOpen(f) {
						printLiveFlow(f)
						f.OpenedPrinted = true
					}
					if f.OpenedPrinted {
						printFlow(f, "idle")
					}
					removeFlow(k)
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

			// skb-out hint first
			if evt == EV_SKB_OUT {
				if !protoAllowed(proto) || (family != AF_INET && family != AF_INET6) || ev.Cookie == 0 {
					continue
				}
				h := l3Info{
					Family: family,
					Proto:  proto,
					Src:    srcKeyFromEvent(ev),
					Sport:  uint16(ev.Sport),
					SrcSc:  srcScopeFromEvent(ev),
					Dst:    dstKeyFromEvent(ev),
					Dport:  uint16(ev.Dport),
					DstSc:  dstScopeFromEvent(ev),
					Seen:   w.now,
				}
				l3ByCookie[ev.Cookie] = h
				for _, f := range flowsByCookie[ev.Cookie] {
					applyL3HintToFlow(f, h)
				}
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
					addFlow(key, f)
				}
			}

			f.LastSeen = w.now
			applyEndpoints(f, ev)

			if h, ok := l3ByCookie[f.Key.Cookie]; ok {
				applyL3HintToFlow(f, h)
			}
			if s, ok := sniC.Get(f.Key.Cookie); ok && f.SNI == "" {
				f.SNI = s
			}

			// accounting by ev.Ret (this is correct for trace_info)
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

			if !f.OpenedPrinted && flowReadyToPrintOpen(f) {
				printLiveFlow(f)
				f.OpenedPrinted = true
			}
		}
	}
}

func setTLSCaptureEnabled(objects *bpfObjects, enabled bool) error {
	if objects.TlsConfigMap == nil {
		return errors.New("tls_config_map is unavailable")
	}
	value := uint8(0)
	if enabled {
		value = 1
	}
	return objects.TlsConfigMap.Put(uint32(0), value)
}

func markTLSCaptureDone(objects *bpfObjects, cookie uint64) error {
	if cookie == 0 {
		return nil
	}
	if objects.TlsDoneMap == nil {
		return errors.New("tls_done_map is unavailable")
	}
	return objects.TlsDoneMap.Put(cookie, uint8(1))
}
