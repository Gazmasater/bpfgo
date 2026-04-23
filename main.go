package main

import (
	"bufio"
	"bytes"
	"context"
	"encoding/binary"
	"errors"
	"flag"
	"fmt"
	"io"
	"log"
	"math/bits"
	"net"
	"net/http"
	_ "net/http/pprof"
	"os"
	"os/signal"
	"path/filepath"
	"runtime"
	"strings"
	"sync"
	"sync/atomic"
	"syscall"
	"time"
	"unsafe"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/link"
	"github.com/cilium/ebpf/perf"
	"github.com/cilium/ebpf/rlimit"
)

//go:generate go run github.com/cilium/ebpf/cmd/bpf2go -target amd64 -type trace_info -type tls_chunk_event bpf trace.c -- -I.

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
	EV_SKB_OUT = 40
)

var (
	flgPerfMB    = flag.Int("perfMB", 8, "perf buffer total budget in MB (divided per-CPU with fallback)")
	flgPprof     = flag.Bool("pprof", true, "enable pprof")
	flgPprofAddr = flag.String("pprofAddr", ":6060", "pprof listen addr")

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
	flgL3TTL       = flag.Duration("l3ttl", 10*time.Second, "TTL for skb-derived L3 hints (cookie->src/dst)")
	flgOpenDelay   = flag.Duration("openDelay", 200*time.Millisecond, "delay OPEN print (max) to wait for skb hint to fill src ip")
	flgL3SweepEach = flag.Int("l3SweepEach", 500, "l3 hint sweep per tick")

	// SNI
	flgSNI        = flag.Bool("sni", true, "parse TLS ClientHello SNI from tls chunk events")
	flgSNITTL     = flag.Duration("sniTTL", 10*time.Minute, "TTL for cookie->SNI cache")
	flgSNIMaxKeep = flag.Int("sniMax", 200000, "max cookie->SNI entries in memory (best effort)")
)

/* ===== basic helpers ===== */

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

// IPv4 u32 from kernel is network-order but looks swapped on little-endian.
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

func isIPv6LinkLocalUnicast(ip [16]byte) bool { return ip[0] == 0xfe && (ip[1]&0xc0) == 0x80 }
func isIPv6LinkLocalMulticast(ip [16]byte) bool {
	return ip[0] == 0xff && (ip[1]&0x0f) == 0x02
}
func needsScope6(ip [16]byte) bool { return isIPv6LinkLocalUnicast(ip) || isIPv6LinkLocalMulticast(ip) }

func isIPv6Loopback(ip [16]byte) bool {
	for i := 0; i < 15; i++ {
		if ip[i] != 0 {
			return false
		}
	}
	return ip[15] == 1 // ::1
}

type ifResolver struct {
	mu sync.Mutex
	m  map[uint32]string
}

func (r *ifResolver) name(ifidx uint32) string {
	if ifidx == 0 {
		return ""
	}
	r.mu.Lock()
	defer r.mu.Unlock()
	if r.m == nil {
		r.m = make(map[uint32]string, 32)
	}
	if s, ok := r.m[ifidx]; ok {
		return s
	}
	ifi, err := net.InterfaceByIndex(int(ifidx))
	if err != nil || ifi == nil || ifi.Name == "" {
		s := fmt.Sprintf("if%d", ifidx)
		r.m[ifidx] = s
		return s
	}
	r.m[ifidx] = ifi.Name
	return ifi.Name
}

var ifr ifResolver

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

func srcScopeFromEvent(ev bpfTraceInfo) uint32 {
	if uint16(ev.Family) == AF_INET6 {
		return uint32(ev.SrcScope)
	}
	return 0
}
func dstScopeFromEvent(ev bpfTraceInfo) uint32 {
	if uint16(ev.Family) == AF_INET6 {
		return uint32(ev.DstScope)
	}
	return 0
}

/* ===== .env loader (no deps) ===== */

func fileExists(path string) bool {
	st, err := os.Stat(path)
	return err == nil && !st.IsDir()
}

func loadDotEnvFile(path string, override bool) error {
	f, err := os.Open(path)
	if err != nil {
		return err
	}
	defer f.Close()
	return loadDotEnvReader(f, override)
}

func loadDotEnvReader(r io.Reader, override bool) error {
	sc := bufio.NewScanner(r)
	for sc.Scan() {
		line := strings.TrimSpace(sc.Text())
		if line == "" || strings.HasPrefix(line, "#") {
			continue
		}
		if i := strings.Index(line, " #"); i >= 0 {
			line = strings.TrimSpace(line[:i])
		}
		if strings.HasPrefix(line, "export ") {
			line = strings.TrimSpace(strings.TrimPrefix(line, "export "))
		}
		i := strings.IndexByte(line, '=')
		if i <= 0 {
			continue
		}
		key := strings.TrimSpace(line[:i])
		val := strings.TrimSpace(line[i+1:])
		if len(val) >= 2 {
			if (val[0] == '"' && val[len(val)-1] == '"') || (val[0] == '\'' && val[len(val)-1] == '\'') {
				val = val[1 : len(val)-1]
			}
		}
		if key == "" {
			continue
		}
		if !override {
			if _, exists := os.LookupEnv(key); exists {
				continue
			}
		}
		_ = os.Setenv(key, val)
	}
	return sc.Err()
}

func loadDotEnvAuto() (string, error) {
	if p, ok := os.LookupEnv("BPFGO_DOTENV"); ok && strings.TrimSpace(p) != "" {
		p = strings.TrimSpace(p)
		if fileExists(p) {
			return p, loadDotEnvFile(p, false)
		}
		return p, fmt.Errorf("BPFGO_DOTENV set but file not found: %s", p)
	}
	if fileExists(".env") {
		return ".env", loadDotEnvFile(".env", false)
	}
	if exe, err := os.Executable(); err == nil {
		dir := filepath.Dir(exe)
		p := filepath.Join(dir, ".env")
		if fileExists(p) {
			return p, loadDotEnvFile(p, false)
		}
	}
	return "", nil
}

func applyEnvToFlags() {
	set := func(flagName, envName string) {
		if v, ok := os.LookupEnv(envName); ok && strings.TrimSpace(v) != "" {
			_ = flag.Set(flagName, strings.TrimSpace(v))
		}
	}

	set("perfMB", "BPFGO_PERF_MB")
	set("pprof", "BPFGO_PPROF")
	set("pprofAddr", "BPFGO_PPROF_ADDR")
	set("ttl", "BPFGO_TTL")
	set("print", "BPFGO_SWEEP")
	set("pid", "BPFGO_ONLY_PID")
	set("comm", "BPFGO_ONLY_COMM")
	set("rw", "BPFGO_RW")
	set("mmsg", "BPFGO_MMSG")

	set("resolve", "BPFGO_RESOLVE")
	set("resolveTTL", "BPFGO_RESOLVE_TTL")
	set("resolveNegTTL", "BPFGO_RESOLVE_NEG_TTL")
	set("resolveWorkers", "BPFGO_RESOLVE_WORKERS")
	set("resolveTimeout", "BPFGO_RESOLVE_TIMEOUT")
	set("resolveQ", "BPFGO_RESOLVE_Q")
	set("hostState", "BPFGO_HOST_STATE")

	set("hostsPrefill", "BPFGO_HOSTS_PREFILL")
	set("hostsFile", "BPFGO_HOSTS_FILE")
	set("hostsTTL", "BPFGO_HOSTS_TTL")

	set("resolveSweepEach", "BPFGO_RESOLVE_SWEEP_EACH")
	set("resolvePokeEach", "BPFGO_RESOLVE_POKE_EACH")

	set("l3ttl", "BPFGO_L3_TTL")
	set("openDelay", "BPFGO_OPEN_DELAY")
	set("l3SweepEach", "BPFGO_L3_SWEEP_EACH")

	set("sni", "BPFGO_SNI")
	set("sniTTL", "BPFGO_SNI_TTL")
	set("sniMax", "BPFGO_SNI_MAX")
}

/* ===== PTR resolver cache ===== */

type dnsKey struct {
	Family uint16
	IP     [16]byte
}

type dnsEntry struct {
	Name    string
	Exp     time.Time
	Pending bool
	Neg     bool
}

type dnsResolver struct {
	mu     sync.Mutex
	m      map[dnsKey]*dnsEntry
	q      chan dnsKey
	ttl    time.Duration
	negTtl time.Duration
	to     time.Duration
	wg     sync.WaitGroup
}

func newDNSResolver(qsize, workers int, ttl, negTtl, timeout time.Duration) *dnsResolver {
	if workers < 1 {
		workers = 1
	}
	if qsize < 64 {
		qsize = 64
	}
	r := &dnsResolver{
		m:      make(map[dnsKey]*dnsEntry, 8192),
		q:      make(chan dnsKey, qsize),
		ttl:    ttl,
		negTtl: negTtl,
		to:     timeout,
	}
	r.wg.Add(workers)
	for i := 0; i < workers; i++ {
		go func() { defer r.wg.Done(); r.worker() }()
	}
	return r
}

func (r *dnsResolver) Close() { close(r.q); r.wg.Wait() }

func ipToNetIP(family uint16, ip [16]byte) net.IP {
	if family == AF_INET {
		return net.IPv4(ip[0], ip[1], ip[2], ip[3]).To4()
	}
	b := make([]byte, 16)
	copy(b, ip[:])
	return net.IP(b)
}

func shouldResolveIP(family uint16, ip [16]byte) bool {
	if isAllZero16(ip) {
		return false
	}
	nip := ipToNetIP(family, ip)
	if nip == nil {
		return false
	}
	if nip.IsLoopback() || nip.IsMulticast() || nip.IsUnspecified() {
		return false
	}
	if family == AF_INET6 && needsScope6(ip) {
		return false
	}
	return true
}

func trimDot(s string) string {
	s = strings.TrimSpace(s)
	for strings.HasSuffix(s, ".") {
		s = strings.TrimSuffix(s, ".")
	}
	return s
}

func (r *dnsResolver) SetKnown(family uint16, ip [16]byte, name string, ttl time.Duration) {
	name = trimDot(name)
	if name == "" {
		return
	}
	k := dnsKey{Family: family, IP: ip}
	r.mu.Lock()
	r.m[k] = &dnsEntry{Name: name, Exp: time.Now().Add(ttl)}
	r.mu.Unlock()
}

func (r *dnsResolver) Get(family uint16, ip [16]byte) (string, bool) {
	if !shouldResolveIP(family, ip) {
		return "", false
	}
	k := dnsKey{Family: family, IP: ip}
	now := time.Now()

	r.mu.Lock()
	defer r.mu.Unlock()

	e := r.m[k]
	if e == nil {
		return "", false
	}
	if now.After(e.Exp) && !e.Pending {
		delete(r.m, k)
		return "", false
	}
	if e.Neg || e.Name == "" {
		return "", false
	}
	return e.Name, true
}

func (r *dnsResolver) Peek(family uint16, ip [16]byte) (name string, pending bool, neg bool, ok bool) {
	if !shouldResolveIP(family, ip) {
		return "", false, false, false
	}
	k := dnsKey{Family: family, IP: ip}
	now := time.Now()

	r.mu.Lock()
	defer r.mu.Unlock()

	e := r.m[k]
	if e == nil {
		return "", false, false, false
	}
	if now.After(e.Exp) && !e.Pending {
		delete(r.m, k)
		return "", false, false, false
	}
	return e.Name, e.Pending, e.Neg, true
}

func (r *dnsResolver) Ensure(family uint16, ip [16]byte) {
	if !shouldResolveIP(family, ip) {
		return
	}
	k := dnsKey{Family: family, IP: ip}
	now := time.Now()

	r.mu.Lock()
	if e := r.m[k]; e != nil {
		if e.Pending || now.Before(e.Exp) {
			r.mu.Unlock()
			return
		}
		e.Pending = true
		e.Name = ""
		e.Neg = false
		r.mu.Unlock()
	} else {
		r.m[k] = &dnsEntry{Pending: true}
		r.mu.Unlock()
	}

	select {
	case r.q <- k:
	default:
		r.mu.Lock()
		if e := r.m[k]; e != nil {
			e.Pending = false
			e.Exp = time.Now().Add(500 * time.Millisecond)
		}
		r.mu.Unlock()
	}
}

func (r *dnsResolver) worker() {
	for k := range r.q {
		ip := ipToNetIP(k.Family, k.IP)
		name := ""
		neg := false
		retrySoon := false

		ctx, cancel := context.WithTimeout(context.Background(), r.to)
		names, err := net.DefaultResolver.LookupAddr(ctx, ip.String())
		cancel()

		if err != nil {
			if dnsErr, ok := err.(*net.DNSError); ok {
				if dnsErr.IsNotFound {
					neg = true
				} else if dnsErr.IsTimeout || dnsErr.IsTemporary {
					retrySoon = true
				} else {
					retrySoon = true
				}
			} else {
				retrySoon = true
			}
		}

		if len(names) > 0 {
			name = trimDot(names[0])
			if name == "" {
				neg = true
				retrySoon = false
			}
		} else {
			if !retrySoon {
				neg = true
			}
		}

		exp := time.Now().Add(r.ttl)
		if neg {
			exp = time.Now().Add(r.negTtl)
		} else if retrySoon {
			exp = time.Now().Add(2 * time.Second)
		}

		r.mu.Lock()
		e := r.m[k]
		if e == nil {
			e = &dnsEntry{}
			r.m[k] = e
		}
		e.Pending = false
		e.Neg = neg
		e.Name = name
		e.Exp = exp
		r.mu.Unlock()
	}
}

func (r *dnsResolver) SweepExpired(limit int) {
	if limit <= 0 {
		return
	}
	now := time.Now()
	n := 0
	r.mu.Lock()
	for k, e := range r.m {
		if !e.Pending && now.After(e.Exp) {
			delete(r.m, k)
			n++
			if n >= limit {
				break
			}
		}
	}
	r.mu.Unlock()
}

func parseHostsPrefill(path string, ttl time.Duration, r *dnsResolver) {
	f, err := os.Open(path)
	if err != nil {
		log.Printf("hostsPrefill: open %s: %v", path, err)
		return
	}
	defer f.Close()

	sc := bufio.NewScanner(f)
	added := 0
	for sc.Scan() {
		line := sc.Text()
		if i := strings.IndexByte(line, '#'); i >= 0 {
			line = line[:i]
		}
		line = strings.TrimSpace(line)
		if line == "" {
			continue
		}
		fields := strings.Fields(line)
		if len(fields) < 2 {
			continue
		}
		ipStr := fields[0]
		name := fields[1]
		ip := net.ParseIP(ipStr)
		if ip == nil {
			continue
		}
		if v4 := ip.To4(); v4 != nil {
			var key [16]byte
			copy(key[:4], v4)
			r.SetKnown(AF_INET, key, name, ttl)
			added++
			continue
		}
		if v6 := ip.To16(); v6 != nil {
			var key [16]byte
			copy(key[:], v6)
			r.SetKnown(AF_INET6, key, name, ttl)
			added++
		}
	}
	if err := sc.Err(); err != nil {
		log.Printf("hostsPrefill: scan %s: %v", path, err)
	}
	log.Printf("hostsPrefill: added=%d from %s", added, path)
}

var dnsr *dnsResolver

/* ===== L3 hint cache ===== */

type l3Info struct {
	Family uint16
	Proto  uint8
	Src    [16]byte
	Sport  uint16
	SrcSc  uint32
	Dst    [16]byte
	Dport  uint16
	DstSc  uint32
	Seen   time.Time
}

func (l l3Info) expired(now time.Time, ttl time.Duration) bool {
	return now.Sub(l.Seen) > ttl
}

/* ===== SNI cache (cookie -> sni) ===== */

type sniEntry struct {
	SNI string
	Exp time.Time
}

type sniCache struct {
	mu   sync.Mutex
	m    map[uint64]sniEntry
	ttl  time.Duration
	maxN int
}

func newSNICache(ttl time.Duration, maxN int) *sniCache {
	if maxN < 1000 {
		maxN = 1000
	}
	return &sniCache{
		m:    make(map[uint64]sniEntry, 8192),
		ttl:  ttl,
		maxN: maxN,
	}
}

func (c *sniCache) Get(cookie uint64) (string, bool) {
	if cookie == 0 {
		return "", false
	}
	now := time.Now()
	c.mu.Lock()
	defer c.mu.Unlock()
	e, ok := c.m[cookie]
	if !ok {
		return "", false
	}
	if now.After(e.Exp) {
		delete(c.m, cookie)
		return "", false
	}
	return e.SNI, e.SNI != ""
}

func (c *sniCache) Put(cookie uint64, sni string) {
	if cookie == 0 || sni == "" {
		return
	}
	now := time.Now()
	c.mu.Lock()
	defer c.mu.Unlock()

	// best-effort cap
	if len(c.m) >= c.maxN {
		// sweep a little
		n := 0
		for k, e := range c.m {
			if now.After(e.Exp) {
				delete(c.m, k)
				n++
				if n >= 1024 {
					break
				}
			}
		}
		// still too big -> random delete a few (map order is random)
		if len(c.m) >= c.maxN {
			n = 0
			for k := range c.m {
				delete(c.m, k)
				n++
				if n >= 1024 {
					break
				}
			}
		}
	}

	c.m[cookie] = sniEntry{SNI: sni, Exp: now.Add(c.ttl)}
}

func (c *sniCache) Sweep(limit int) {
	if limit <= 0 {
		return
	}
	now := time.Now()
	n := 0
	c.mu.Lock()
	for k, e := range c.m {
		if now.After(e.Exp) {
			delete(c.m, k)
			n++
			if n >= limit {
				break
			}
		}
	}
	c.mu.Unlock()
}

/* ===== endpoint printing ===== */

func specialAlias(family uint16, ip [16]byte) (string, bool) {
	if family == AF_INET {
		if ip[0] == 127 && ip[1] == 0 && ip[2] == 0 && ip[3] == 1 {
			return "localhost", true
		}
		if ip[0] == 127 && ip[1] == 0 && ip[2] == 0 && ip[3] == 53 {
			return "dnsstub", true
		}
		if ip[0] == 10 && ip[1] == 0 && ip[2] == 2 && ip[3] == 2 {
			return "vboxgw", true
		}
		if ip[0] == 10 && ip[1] == 0 && ip[2] == 2 && ip[3] == 3 {
			return "vboxdns", true
		}
	}
	if family == AF_INET6 && isIPv6Loopback(ip) {
		return "localhost", true
	}
	return "", false
}

func addrStr(family uint16, ip [16]byte, scope uint32) string {
	if isAllZero16(ip) {
		return "*"
	}
	if family == AF_INET6 {
		s := fmtIPv6Full(ip)
		if needsScope6(ip) && scope != 0 {
			s += "%" + ifr.name(scope)
		}
		return s
	}
	return fmtIPv4FromKey(ip)
}

func aliasForIP(family uint16, ip [16]byte) string {
	if isAllZero16(ip) {
		return "any"
	}
	if a, ok := specialAlias(family, ip); ok {
		return a
	}
	if dnsr == nil {
		return "?"
	}
	if !shouldResolveIP(family, ip) {
		return "skip"
	}
	if h, ok := dnsr.Get(family, ip); ok && h != "" {
		return h
	}
	if name, pending, neg, ok := dnsr.Peek(family, ip); ok {
		if name != "" && !neg {
			return name
		}
		if pending {
			if *flgHostState {
				return "pending"
			}
			return "?"
		}
		if neg {
			if *flgHostState {
				return "no-ptr"
			}
			return "?"
		}
	}
	dnsr.Ensure(family, ip)
	if *flgHostState {
		return "miss"
	}
	return "?"
}

func fmtEndpointAll(family uint16, ip [16]byte, port uint16, scope uint32, proto uint8, alias string) string {
	isICMP := proto == IPPROTO_ICMP || proto == IPPROTO_ICMPV6
	a := addrStr(family, ip, scope)
	if alias == "" {
		alias = "?"
	}
	if isICMP {
		return fmt.Sprintf("%s(%s)", a, alias)
	}
	if family == AF_INET6 && !isAllZero16(ip) {
		return fmt.Sprintf("[%s](%s):%d", a, alias, port)
	}
	return fmt.Sprintf("%s(%s):%d", a, alias, port)
}

/* ===== FLOW ===== */

type FlowKey struct {
	TGID   uint32
	Cookie uint64
	Proto  uint8
	Family uint16

	PeerMode uint8
	Rport    uint16
	Remote   [16]byte
	Rscope   uint32
}

type Flow struct {
	Key  FlowKey
	Comm string

	Local      [16]byte
	Lport      uint16
	LocalScope uint32

	Remote      [16]byte
	Rport       uint16
	RemoteScope uint32

	FirstSeen time.Time
	LastSeen  time.Time

	InBytes  uint64
	OutBytes uint64
	InPkts   uint64
	OutPkts  uint64

	OpenedPrinted bool
	GenStart      uint64

	RemoteHost string
	SNI        string
}

func makeKey(ev bpfTraceInfo) FlowKey {
	k := FlowKey{
		TGID:   ev.Tgid,
		Cookie: ev.Cookie,
		Proto:  uint8(ev.Proto),
		Family: uint16(ev.Family),
	}

	evt := uint8(ev.Event)

	if k.Proto == IPPROTO_UDP {
		var remote [16]byte
		var rport uint16
		var rscope uint32

		if isRecv(evt) {
			remote = srcKeyFromEvent(ev)
			rport = uint16(ev.Sport)
			rscope = srcScopeFromEvent(ev)
		} else {
			remote = dstKeyFromEvent(ev)
			rport = uint16(ev.Dport)
			rscope = dstScopeFromEvent(ev)
		}

		if rport != 0 && !isAllZero16(remote) {
			k.PeerMode = 1
			k.Remote = remote
			k.Rport = rport
			if k.Family == AF_INET6 && needsScope6(remote) && rscope != 0 {
				k.Rscope = rscope
			}
		}
	}

	if k.Proto == IPPROTO_ICMP || k.Proto == IPPROTO_ICMPV6 {
		var remote [16]byte
		var rscope uint32
		if isRecv(evt) {
			remote = srcKeyFromEvent(ev)
			rscope = srcScopeFromEvent(ev)
		} else {
			remote = dstKeyFromEvent(ev)
			rscope = dstScopeFromEvent(ev)
		}
		if !isAllZero16(remote) {
			k.PeerMode = 1
			k.Remote = remote
			if k.Family == AF_INET6 && needsScope6(remote) && rscope != 0 {
				k.Rscope = rscope
			}
		}
	}

	return k
}

func applyEndpoints(f *Flow, ev bpfTraceInfo) {
	evt := uint8(ev.Event)

	var localIP, remoteIP [16]byte
	var lport, rport uint16
	var localScope, remoteScope uint32

	switch {
	case isSend(evt) || evt == EV_CONNECT:
		localIP = srcKeyFromEvent(ev)
		remoteIP = dstKeyFromEvent(ev)
		lport = uint16(ev.Sport)
		rport = uint16(ev.Dport)
		localScope = srcScopeFromEvent(ev)
		remoteScope = dstScopeFromEvent(ev)

	case isRecv(evt):
		localIP = dstKeyFromEvent(ev)
		remoteIP = srcKeyFromEvent(ev)
		lport = uint16(ev.Dport)
		rport = uint16(ev.Sport)
		localScope = dstScopeFromEvent(ev)
		remoteScope = srcScopeFromEvent(ev)

	case evt == EV_BINDOK:
		localIP = srcKeyFromEvent(ev)
		lport = uint16(ev.Sport)
		localScope = srcScopeFromEvent(ev)

	case evt == EV_ACCEPT:
		localIP = dstKeyFromEvent(ev)
		remoteIP = srcKeyFromEvent(ev)
		lport = uint16(ev.Dport)
		rport = uint16(ev.Sport)
		localScope = dstScopeFromEvent(ev)
		remoteScope = srcScopeFromEvent(ev)
	}

	if f.Lport == 0 && lport != 0 {
		f.Lport = lport
	}
	if isAllZero16(f.Local) && !isAllZero16(localIP) {
		f.Local = localIP
	}
	if f.LocalScope == 0 && localScope != 0 && needsScope6(localIP) {
		f.LocalScope = localScope
	}

	if f.Rport == 0 && rport != 0 {
		f.Rport = rport
	}
	if isAllZero16(f.Remote) && !isAllZero16(remoteIP) {
		f.Remote = remoteIP
	}
	if f.RemoteScope == 0 && remoteScope != 0 && needsScope6(remoteIP) {
		f.RemoteScope = remoteScope
	}
}

func applyL3HintToFlow(f *Flow, h l3Info) {
	if h.Family != f.Key.Family || h.Proto != f.Key.Proto {
		return
	}

	if !isAllZero16(f.Remote) {
		if f.Rport != 0 && f.Rport == h.Dport && bytes.Equal(f.Remote[:], h.Dst[:]) {
			if isAllZero16(f.Local) && !isAllZero16(h.Src) {
				f.Local = h.Src
				f.LocalScope = h.SrcSc
			}
			if f.Lport == 0 && h.Sport != 0 {
				f.Lport = h.Sport
			}
			return
		}
		if f.Rport != 0 && f.Rport == h.Sport && bytes.Equal(f.Remote[:], h.Src[:]) {
			if isAllZero16(f.Local) && !isAllZero16(h.Dst) {
				f.Local = h.Dst
				f.LocalScope = h.DstSc
			}
			if f.Lport == 0 && h.Dport != 0 {
				f.Lport = h.Dport
			}
			return
		}
	}

	if isAllZero16(f.Local) && !isAllZero16(h.Src) {
		f.Local = h.Src
		f.LocalScope = h.SrcSc
		if f.Lport == 0 && h.Sport != 0 {
			f.Lport = h.Sport
		}
	}
}

func flowReadyToOpenBase(f *Flow) bool {
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

func flowReadyToPrintOpen(f *Flow) bool {
	if !flowReadyToOpenBase(f) {
		return false
	}
	if isAllZero16(f.Local) && time.Since(f.FirstSeen) < *flgOpenDelay {
		return false
	}
	return true
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

func remoteAliasCached(f *Flow) string {
	if f.RemoteHost != "" {
		return f.RemoteHost
	}
	a := aliasForIP(f.Key.Family, f.Remote)
	switch a {
	case "", "?", "any", "skip", "pending", "no-ptr", "miss":
		return a
	default:
		f.RemoteHost = a
		return a
	}
}

func sniNote(f *Flow) string {
	if f.SNI == "" {
		return ""
	}
	return " sni=" + f.SNI
}

func printOpen(f *Flow) {
	lAlias := aliasForIP(f.Key.Family, f.Local)
	rAlias := remoteAliasCached(f)

	fmt.Printf("OPEN  %-5s pid=%d(%s) cookie=%d  %s -> %s%s%s%s\n",
		protoStr(f.Key.Proto),
		f.Key.TGID, f.Comm, f.Key.Cookie,
		fmtEndpointAll(f.Key.Family, f.Local, f.Lport, f.LocalScope, f.Key.Proto, lAlias),
		fmtEndpointAll(f.Key.Family, f.Remote, f.Rport, f.RemoteScope, f.Key.Proto, rAlias),
		sniNote(f),
		incompleteNote(f),
		maybeLostNote(f),
	)
}

func printClose(f *Flow, reason string) {
	lAlias := aliasForIP(f.Key.Family, f.Local)
	rAlias := remoteAliasCached(f)

	age := time.Since(f.FirstSeen).Truncate(time.Millisecond)
	fmt.Printf("CLOSE %-5s pid=%d(%s) cookie=%d  %s -> %s  out=%dB/%dp in=%dB/%dp  age=%s reason=%s%s%s%s\n",
		protoStr(f.Key.Proto),
		f.Key.TGID, f.Comm, f.Key.Cookie,
		fmtEndpointAll(f.Key.Family, f.Local, f.Lport, f.LocalScope, f.Key.Proto, lAlias),
		fmtEndpointAll(f.Key.Family, f.Remote, f.Rport, f.RemoteScope, f.Key.Proto, rAlias),
		f.OutBytes, f.OutPkts, f.InBytes, f.InPkts,
		age, reason,
		sniNote(f),
		incompleteNote(f),
		maybeLostNote(f),
	)
}

/* ===== perf reader: total budget -> per-CPU + fallback ===== */

func openPerfReaderTotalBudget(events *ebpf.Map, totalMB int) (*perf.Reader, int, error) {
	totalBytes := totalMB * 1024 * 1024
	if totalBytes < 256*1024 {
		totalBytes = 256 * 1024
	}

	nCPU := runtime.NumCPU()
	perCPU := totalBytes / nCPU
	if perCPU < 256*1024 {
		perCPU = 256 * 1024
	}

	page := os.Getpagesize()
	pages := perCPU / page
	if pages < 8 {
		pages = 8
	}
	p2 := 1 << (bits.Len(uint(pages)) - 1)
	try := p2 * page

	var rd *perf.Reader
	var err error
	for try >= 256*1024 {
		rd, err = perf.NewReader(events, try)
		if err == nil {
			return rd, try, nil
		}
		if strings.Contains(err.Error(), "cannot allocate memory") || strings.Contains(err.Error(), "can't mmap") {
			try /= 2
			continue
		}
		break
	}
	return nil, 0, err
}

/* ===== TLS SNI parser from chunk stream =====
   We parse TLS record (Handshake) ClientHello and extract SNI (server_name).
   Works on partial chunks: we keep small per-cookie reassembly buffer.
*/

type tlsReasm struct {
	buf   []byte
	seen  time.Time
	done  bool
	sni   string
	stage string
}

type tlsAssembler struct {
	mu        sync.Mutex
	byCookie  map[uint64]*tlsReasm
	ttl       time.Duration
	maxCookie int
}

func newTLSAssembler(ttl time.Duration, maxN int) *tlsAssembler {
	if maxN < 1000 {
		maxN = 1000
	}
	return &tlsAssembler{
		byCookie:  make(map[uint64]*tlsReasm, 8192),
		ttl:       ttl,
		maxCookie: maxN,
	}
}

func (a *tlsAssembler) Sweep(limit int) {
	if limit <= 0 {
		return
	}
	now := time.Now()
	n := 0
	a.mu.Lock()
	for k, v := range a.byCookie {
		if now.Sub(v.seen) > a.ttl {
			delete(a.byCookie, k)
			n++
			if n >= limit {
				break
			}
		}
	}
	a.mu.Unlock()
}

func (a *tlsAssembler) Push(cookie uint64, data []byte) (sni string, ok bool) {
	if cookie == 0 || len(data) == 0 {
		return "", false
	}

	a.mu.Lock()
	defer a.mu.Unlock()

	ra := a.byCookie[cookie]
	if ra == nil {
		ra = &tlsReasm{buf: make([]byte, 0, 2048)}
		a.byCookie[cookie] = ra
	}
	ra.seen = time.Now()
	if ra.done {
		return ra.sni, ra.sni != ""
	}

	// cap buffer to avoid memory blow
	const maxBuf = 16 * 1024
	if len(ra.buf)+len(data) > maxBuf {
		// drop oldest, keep last part
		need := (len(ra.buf) + len(data)) - maxBuf
		if need < len(ra.buf) {
			ra.buf = ra.buf[need:]
		} else {
			ra.buf = ra.buf[:0]
		}
	}
	ra.buf = append(ra.buf, data...)

	sni = tryParseSNIFromTLS(ra.buf)
	if sni != "" {
		ra.done = true
		ra.sni = sni
		return sni, true
	}
	return "", false
}

// minimal TLS ClientHello SNI extraction (best-effort)
func tryParseSNIFromTLS(b []byte) string {
	// TLS record header: 5 bytes
	// content_type(1)=0x16, version(2), length(2)
	// handshake: type(1)=0x01, length(3), ...
	i := 0
	for {
		if len(b)-i < 5 {
			return ""
		}
		ct := b[i]
		if ct != 0x16 { // Handshake
			// skip this record
			if len(b)-i < 5 {
				return ""
			}
			recLen := int(binary.BigEndian.Uint16(b[i+3 : i+5]))
			i += 5 + recLen
			if i >= len(b) {
				return ""
			}
			continue
		}
		recLen := int(binary.BigEndian.Uint16(b[i+3 : i+5]))
		if len(b)-i < 5+recLen {
			return ""
		}
		rec := b[i+5 : i+5+recLen]

		// handshake header inside record
		if len(rec) < 4 {
			return ""
		}
		if rec[0] != 0x01 { // ClientHello
			i += 5 + recLen
			continue
		}
		hsLen := int(rec[1])<<16 | int(rec[2])<<8 | int(rec[3])
		if len(rec) < 4+hsLen {
			return ""
		}
		ch := rec[4 : 4+hsLen]

		// ClientHello:
		// version(2) + random(32) + session_id_len(1)+session_id + cipher_suites_len(2)+... + comp_len(1)+... + ext_len(2)+ext...
		p := 0
		if len(ch) < p+2+32 {
			return ""
		}
		p += 2 + 32

		if len(ch) < p+1 {
			return ""
		}
		sidLen := int(ch[p])
		p += 1
		if len(ch) < p+sidLen {
			return ""
		}
		p += sidLen

		if len(ch) < p+2 {
			return ""
		}
		csLen := int(binary.BigEndian.Uint16(ch[p : p+2]))
		p += 2
		if len(ch) < p+csLen {
			return ""
		}
		p += csLen

		if len(ch) < p+1 {
			return ""
		}
		compLen := int(ch[p])
		p += 1
		if len(ch) < p+compLen {
			return ""
		}
		p += compLen

		if len(ch) < p+2 {
			return ""
		}
		extLen := int(binary.BigEndian.Uint16(ch[p : p+2]))
		p += 2
		if len(ch) < p+extLen {
			return ""
		}
		ext := ch[p : p+extLen]

		// Extensions: [type(2), len(2), data(len)]...
		q := 0
		for q+4 <= len(ext) {
			et := binary.BigEndian.Uint16(ext[q : q+2])
			el := int(binary.BigEndian.Uint16(ext[q+2 : q+4]))
			q += 4
			if q+el > len(ext) {
				return ""
			}
			if et == 0x0000 { // server_name
				sni := parseSNIExt(ext[q : q+el])
				if sni != "" {
					return sni
				}
				return ""
			}
			q += el
		}
		return ""
	}
}

func parseSNIExt(b []byte) string {
	// server_name ext:
	// list_len(2) + [name_type(1)=0, name_len(2), name(name_len)]...
	if len(b) < 2 {
		return ""
	}
	listLen := int(binary.BigEndian.Uint16(b[:2]))
	if len(b) < 2+listLen {
		return ""
	}
	p := 2
	end := 2 + listLen
	for p+3 <= end {
		nt := b[p]
		nl := int(binary.BigEndian.Uint16(b[p+1 : p+3]))
		p += 3
		if p+nl > end {
			return ""
		}
		if nt == 0 { // host_name
			name := string(b[p : p+nl])
			name = strings.TrimSpace(name)
			// простая валидация
			if name != "" && !strings.ContainsAny(name, " \t\r\n") {
				return name
			}
		}
		p += nl
	}
	return ""
}

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

	attach("net", "net_dev_queue", objs.TraceNetDevQueue)

	// main flow perf reader
	rd, perCPUBytes, err := openPerfReaderTotalBudget(objs.TraceEvents, *flgPerfMB)
	if err != nil {
		log.Fatalf("perf.NewReader(trace_events): %v", err)
	}
	defer rd.Close()

	log.Printf("perf ring trace_events per-cpu=%dKB total~=%dMB cpus=%d",
		perCPUBytes/1024, (perCPUBytes*runtime.NumCPU())/(1024*1024), runtime.NumCPU())

	// --- TLS reader (SNI) ---
	// IMPORTANT:
	// In your generated bpfMaps there is no tls_events perf map right now.
	// Add it in trace.c and regenerate bpf2go.
	//
	// Once you add map like: TlsEvents *ebpf.Map `ebpf:"tls_events"`,
	// rename here accordingly.
	var tlsRD *perf.Reader
	var tlsAssembler *tlsAssembler
	sniC := newSNICache(*flgSNITTL, *flgSNIMaxKeep)

	// TODO: if you expose map in bpfMaps as TlsEvents, uncomment:
	// if *flgSNI && objs.TlsEvents != nil {
	//     tlsRD, _, err = openPerfReaderTotalBudget(objs.TlsEvents, max(1, *flgPerfMB/2))
	//     if err != nil { log.Printf("tls perf reader disabled: %v", err); tlsRD=nil }
	// }

	_ = tlsRD
	if *flgSNI {
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
	l3ByCookie := make(map[uint64]l3Info, 8192)

	ticker := time.NewTicker(*flgSweep)
	defer ticker.Stop()

	log.Println("OPEN/CLOSE (TCP/UDP/ICMP) + PTR + skb-hint (+SNI if tls_events enabled). Ctrl+C to exit")

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
				if h, ok := l3ByCookie[cookie]; ok {
					applyL3HintToFlow(f, h)
				}
				if s, ok := sniC.Get(cookie); ok && f.SNI == "" {
					f.SNI = s
				}

				if dropZeroFlow(f) {
					delete(flows, k)
					continue
				}
				if !f.OpenedPrinted && flowReadyToPrintOpen(f) {
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
			if tlsRD != nil {
				_ = tlsRD.Close()
			}
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
				// update live flows
				for _, f := range flows {
					if f.Key.Cookie == ev.Cookie && f.SNI == "" {
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
						delete(flows, k)
						continue
					}
					if !f.OpenedPrinted && flowReadyToPrintOpen(f) {
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
				for _, f := range flows {
					if f.Key.Cookie == ev.Cookie {
						applyL3HintToFlow(f, h)
					}
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
					flows[key] = f
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
				printOpen(f)
				f.OpenedPrinted = true
			}
		}
	}
}
