import "net"

// Конвертация IPv6 из 4 LE-uint32 в net.IP
func IPv6FromLEWords(words [4]uint32) net.IP {
	ip := make(net.IP, 16)
	for i := 0; i < 4; i++ {
		ip[i*4+0] = byte(words[i])
		ip[i*4+1] = byte(words[i] >> 8)
		ip[i*4+2] = byte(words[i] >> 16)
		ip[i*4+3] = byte(words[i] >> 24)
	}
	return ip
}

// Аналогичная функция, если нужна с другим названием
func IPv6FromLEWords4(words [4]uint32) net.IP {
	ip := make(net.IP, 16)
	for i := 0; i < 4; i++ {
		ip[i*4+0] = byte(words[i])
		ip[i*4+1] = byte(words[i] >> 8)
		ip[i*4+2] = byte(words[i] >> 16)
		ip[i*4+3] = byte(words[i] >> 24)
	}
	return ip
}

