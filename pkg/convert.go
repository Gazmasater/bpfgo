package pkg

import (
	"bytes"
	"net"
	"unsafe"
)

func ResolveIP(ip net.IP) string {
	names, err := net.LookupAddr(ip.String())
	if err != nil || len(names) == 0 {
		return "Unknown"
	}
	return names[0]
}

func Int8ToString(arr [64]int8) string {
	b := unsafe.Slice((*byte)(unsafe.Pointer(&arr[0])), len(arr))
	return FastBytes2String(bytes.Trim(b, "\x00"))
}

func FastBytes2String(b []byte) string {
	if len(b) == 0 {
		return ""
	}
	return unsafe.String(unsafe.SliceData(b), len(b))
}
