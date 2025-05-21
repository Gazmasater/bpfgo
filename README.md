
type bpfTraceInfo struct {
	SrcIP   struct{ S_addr uint32 }
	DstIP   struct{ S_addr uint32 }
	SrcIP6  [4]uint32
	DstIP6  [4]uint32
	Pid     uint32
	Proto   uint32
	Sport   uint16
	Dport   uint16
	Family  uint16
	Sysexit uint8
	State   uint8
	Comm    [32]int8
}

