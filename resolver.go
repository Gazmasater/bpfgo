package main

import (
	"bufio"
	"context"
	"log"
	"net"
	"os"
	"strings"
	"sync"
	"time"
)

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
