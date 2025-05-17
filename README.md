type bpfTraceInfo struct {
	SsrcIP struct {
		SinFamily uint16
		SinPort   uint16
		SinAddr   struct{ S_addr uint32 }
		Pad       [8]uint8
	}
	DdstIP struct {
		SinFamily uint16
		SinPort   uint16
		SinAddr   struct{ S_addr uint32 }
		Pad       [8]uint8
	}
	SrcIP   struct{ S_addr uint32 }
	DstIP   struct{ S_addr uint32 }
	SrcIP6  [4]uint32
	DstIP6  [4]uint32
	Pid     uint32
	Proto   uint32
	Sysexit uint16
	Sport   uint16
	Dport   uint16
	State   uint16
	Family  uint16
	Comm    [32]int8
	_       [2]byte
}




