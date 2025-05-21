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
	port := event.Dport
	pid := event.Pid
	ipv6 := pkg.IPv6FromLEWords(
		IPv6BytesToWords(event.DstIP6.In6U.U6Addr8),
	)

	fmt.Printf("SENDTO IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
		pid,
		ipv6.String(),
		port,
		pkg.Int8ToString(event.Comm),
	)
}

