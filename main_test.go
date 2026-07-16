package main

import (
	"testing"
	"time"
)

func completeTestFlow(proto uint8) *Flow {
	f := &Flow{
		Key:       FlowKey{Proto: proto, Family: AF_INET},
		Lport:     12345,
		Rport:     443,
		FirstSeen: time.Now().Add(-time.Second),
	}
	f.Local[12] = 127
	f.Local[15] = 1
	f.Remote[12] = 127
	f.Remote[15] = 1
	return f
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
