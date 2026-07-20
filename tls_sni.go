package main

import (
	"encoding/binary"
	"strings"
	"sync"
	"time"
)

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
