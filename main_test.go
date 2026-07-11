package main

import (
	"encoding/binary"
	"io"
	"os"
	"strings"
	"testing"
	"time"
)

func ipv4U32(a, b, c, d byte) uint32 {
	return binary.LittleEndian.Uint32([]byte{a, b, c, d})
}

func ipv4Key(a, b, c, d byte) (k [16]byte) {
	k[0], k[1], k[2], k[3] = a, b, c, d
	return k
}

func completeTestFlow(proto uint8) *Flow {
	f := &Flow{
		Key:       FlowKey{Proto: proto, Family: AF_INET},
		Lport:     12345,
		Rport:     443,
		FirstSeen: time.Now().Add(-time.Second),
	}
	f.Local = ipv4Key(127, 0, 0, 1)
	f.Remote = ipv4Key(127, 0, 0, 1)
	return f
}

func TestApplyEndpointsUsesProcessPerspective(t *testing.T) {
	tests := []struct {
		name  string
		event uint8
		proto uint8
		src   uint32
		dst   uint32
		sport uint16
		dport uint16
	}{
		{"send", EV_SENDTO, IPPROTO_UDP, ipv4U32(10, 0, 0, 2), ipv4U32(8, 8, 8, 8), 53000, 53},
		{"connect", EV_CONNECT, IPPROTO_TCP, ipv4U32(10, 0, 0, 2), ipv4U32(1, 1, 1, 1), 41000, 443},
		// Receive and accept events arrive as peer -> process and must be reversed for display.
		{"recv", EV_RECVFROM, IPPROTO_UDP, ipv4U32(8, 8, 8, 8), ipv4U32(10, 0, 0, 2), 53, 53000},
		{"accept", EV_ACCEPT, IPPROTO_TCP, ipv4U32(192, 0, 2, 10), ipv4U32(10, 0, 0, 2), 55000, 8080},
	}

	for _, tc := range tests {
		t.Run(tc.name, func(t *testing.T) {
			ev := bpfTraceInfo{Family: AF_INET, Proto: tc.proto, Event: tc.event, SrcIp4: tc.src, DstIp4: tc.dst, Sport: tc.sport, Dport: tc.dport}
			f := &Flow{Key: FlowKey{Family: AF_INET, Proto: tc.proto}}
			applyEndpoints(f, ev)

			wantLocal, wantRemote := srcKeyFromEvent(ev), dstKeyFromEvent(ev)
			wantLport, wantRport := tc.sport, tc.dport
			if isRecv(tc.event) || tc.event == EV_ACCEPT {
				wantLocal, wantRemote = wantRemote, wantLocal
				wantLport, wantRport = wantRport, wantLport
			}
			if f.Local != wantLocal || f.Remote != wantRemote || f.Lport != wantLport || f.Rport != wantRport {
				t.Fatalf("wrong endpoints: local=%s:%d remote=%s:%d", addrStr(AF_INET, f.Local, 0), f.Lport, addrStr(AF_INET, f.Remote, 0), f.Rport)
			}
		})
	}
}

func TestUDPKeyUsesSameRemotePeerForSendAndReceive(t *testing.T) {
	send := bpfTraceInfo{Tgid: 7, Cookie: 9, Family: AF_INET, Proto: IPPROTO_UDP, Event: EV_SENDTO, SrcIp4: ipv4U32(10, 0, 0, 2), DstIp4: ipv4U32(8, 8, 8, 8), Sport: 53000, Dport: 53}
	recv := bpfTraceInfo{Tgid: 7, Cookie: 9, Family: AF_INET, Proto: IPPROTO_UDP, Event: EV_RECVFROM, SrcIp4: send.DstIp4, DstIp4: send.SrcIp4, Sport: send.Dport, Dport: send.Sport}

	ks, kr := makeKey(send), makeKey(recv)
	if ks != kr {
		t.Fatalf("send and receive for one UDP peer produced different keys: %#v != %#v", ks, kr)
	}
	if ks.PeerMode != 1 || ks.Remote != ipv4Key(8, 8, 8, 8) || ks.Rport != 53 {
		t.Fatalf("wrong UDP peer key: %#v", ks)
	}

	send.DstIp4 = ipv4U32(1, 1, 1, 1)
	if makeKey(send) == ks {
		t.Fatal("different UDP peers must not collapse into one flow")
	}
}

func captureStdout(t *testing.T, fn func()) string {
	t.Helper()
	r, w, err := os.Pipe()
	if err != nil {
		t.Fatal(err)
	}
	old := os.Stdout
	os.Stdout = w
	defer func() { os.Stdout = old }()

	fn()
	if err := w.Close(); err != nil {
		t.Fatal(err)
	}
	b, err := io.ReadAll(r)
	if err != nil {
		t.Fatal(err)
	}
	_ = r.Close()
	return string(b)
}

func TestTerminalOpenCloseFormatAndCounters(t *testing.T) {
	f := &Flow{
		Key:       FlowKey{TGID: 42, Cookie: 99, Proto: IPPROTO_TCP, Family: AF_INET},
		Comm:      "curl",
		Local:     ipv4Key(10, 0, 0, 2),
		Lport:     41000,
		Remote:    ipv4Key(93, 184, 216, 34),
		Rport:     443,
		FirstSeen: time.Now().Add(-1500 * time.Millisecond),
		OutBytes:  123,
		OutPkts:   2,
		InBytes:   456,
		InPkts:    3,
	}
	out := captureStdout(t, func() {
		printOpen(f)
		printClose(f, "close()")
	})

	wants := []string{
		"OPEN  TCP   pid=42(curl) cookie=99",
		"CLOSE TCP   pid=42(curl) cookie=99",
		"10.0.0.2(?):41000 -> 93.184.216.34(?):443",
		"out=123B/2p in=456B/3p",
		"reason=close()",
	}
	for _, want := range wants {
		if !strings.Contains(out, want) {
			t.Fatalf("terminal output missing %q:\n%s", want, out)
		}
	}
	if strings.Contains(out, "%!") {
		t.Fatalf("fmt argument mismatch in terminal output:\n%s", out)
	}
}

func TestIPv6EndpointFormatting(t *testing.T) {
	ip := [16]byte{0x20, 0x01, 0x0d, 0xb8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
	got := fmtEndpointAll(AF_INET6, ip, 443, 0, IPPROTO_TCP, "example")
	if got != "[2001:db8:0:0:0:0:0:1](example):443" {
		t.Fatalf("unexpected IPv6 endpoint: %s", got)
	}
}

func TestFlowReadySuppressesZeroTrafficUDP(t *testing.T) {
	f := completeTestFlow(IPPROTO_UDP)
	if flowReadyToPrintOpen(f) {
		t.Fatal("zero-traffic UDP route probe must not be printed")
	}
	f.OutPkts = 1
	f.OutBytes = 10
	if !flowReadyToPrintOpen(f) {
		t.Fatal("UDP flow with traffic must be printed")
	}
}

func TestFlowReadyKeepsTCPConnectVisible(t *testing.T) {
	f := completeTestFlow(IPPROTO_TCP)
	if !flowReadyToPrintOpen(f) {
		t.Fatal("TCP connect must remain visible before payload traffic")
	}
}
