package pkg

import (
	"bytes"
	"fmt"
	"net"
	"unsafe"

	"github.com/miekg/dns"
)

func ResolveIP(ip net.IP) string {
	names, err := net.LookupAddr(ip.String())
	if err != nil || len(names) == 0 {
		return "Unknown"
	}
	return names[0]
}

func ResolveIP_n(ip net.IP) (string, error) {
	// Получаем обратную запись для PTR-запроса
	ptrName, err := dns.ReverseAddr(ip.String())
	if err != nil {
		return "", fmt.Errorf("не удалось сформировать PTR-запрос: %v", err)
	}

	// Создаём DNS-запрос
	m := new(dns.Msg)
	m.SetQuestion(ptrName, dns.TypePTR)

	// Используем Google DNS
	c := new(dns.Client)
	r, _, err := c.Exchange(m, "8.8.8.8:53")
	if err != nil {
		return "", fmt.Errorf("ошибка DNS-запроса: %v", err)
	}

	// Обрабатываем ответ
	for _, ans := range r.Answer {
		if ptr, ok := ans.(*dns.PTR); ok {
			return ptr.Ptr, nil
		}
	}

	return "", fmt.Errorf("доменное имя для IP %s не найдено", ip.String())
}

func Int8ToString(arr [64]int8) string {
	b := unsafe.Slice((*byte)(unsafe.Pointer(&arr[0])), len(arr))
	b = bytes.Trim(b, "\x00")
	if len(b) == 0 {
		return ""
	}
	return unsafe.String(unsafe.SliceData(b), len(b))
}
