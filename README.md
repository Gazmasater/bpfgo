// Преобразование 16 байт в 4 слова uint32 little-endian
func IPv6BytesToWords(addr [16]uint8) [4]uint32 {
	var words [4]uint32
	for i := 0; i < 4; i++ {
		words[i] = uint32(addr[i*4]) |
			uint32(addr[i*4+1])<<8 |
			uint32(addr[i*4+2])<<16 |
			uint32(addr[i*4+3])<<24
	}
	return words
}




if family == 10 {
	fmt.Printf("LOOKUP SRC IPv6=%s\n",
		pkg.IPv6FromLEWords(IPv6BytesToWords(event.SrcIP6)),
	)
	fmt.Printf("LOOKUP DST IPv6=%s\n",
		pkg.IPv6FromLEWords(IPv6BytesToWords(event.DstIP6)),
	)
	fmt.Printf("LOOKUP SPORT=%d  DPORT=%d PROTO=%d\n", event.Sport, event.Dport, event.Proto)
}
