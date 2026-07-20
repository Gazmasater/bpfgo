package main

import (
	"bytes"
	"fmt"
	"sync"
	"sync/atomic"
	"time"
)

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
	// libc/getaddrinfo may briefly connect UDP sockets to candidate addresses
	// only to select a source route. Those sockets never carry traffic and are
	// not useful network flows, so don't print them as OPEN.
	if f.Key.Proto == IPPROTO_UDP && f.InPkts == 0 && f.OutPkts == 0 {
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

func printLiveFlow(f *Flow) {
	lAlias := aliasForIP(f.Key.Family, f.Local)
	rAlias := remoteAliasCached(f)

	fmt.Printf("LIVE  %-5s pid=%d(%s) cookie=%d  %s -> %s%s%s%s\n",
		protoStr(f.Key.Proto),
		f.Key.TGID, f.Comm, f.Key.Cookie,
		fmtEndpointAll(f.Key.Family, f.Local, f.Lport, f.LocalScope, f.Key.Proto, lAlias),
		fmtEndpointAll(f.Key.Family, f.Remote, f.Rport, f.RemoteScope, f.Key.Proto, rAlias),
		sniNote(f),
		incompleteNote(f),
		maybeLostNote(f),
	)
}

func printFlow(f *Flow, reason string) {
	lAlias := aliasForIP(f.Key.Family, f.Local)
	rAlias := remoteAliasCached(f)

	age := time.Since(f.FirstSeen).Truncate(time.Millisecond)
	fmt.Printf("FLOW  %-5s pid=%d(%s) cookie=%d  %s <-> %s  out=%dB/%dp in=%dB/%dp  age=%s end=%s%s%s%s\n",
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
