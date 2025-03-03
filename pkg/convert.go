package pkg

import (
	"bytes"
	"net"
)

func ResolveIP(ip net.IP) string {
	names, err := net.LookupAddr(ip.String())
	if err != nil || len(names) == 0 {
		return "Unknown"
	}
	return names[0]
}

func Int8ToString(arr [64]int8) string {
	byteArr := make([]byte, len(arr))
	for i, v := range arr {
		byteArr[i] = byte(v)
	}

	return string(bytes.Trim(byteArr, "\x00"))
}
