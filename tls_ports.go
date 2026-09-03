package main

import (
	"fmt"
	"sort"
	"strconv"
	"strings"
)

const maxTLSPorts = 64

func parseTLSPorts(raw string) ([]uint16, error) {
	parts := strings.Split(raw, ",")
	ports := make([]uint16, 0, len(parts))
	seen := make(map[uint16]struct{}, len(parts))

	for _, part := range parts {
		part = strings.TrimSpace(part)
		if part == "" {
			return nil, fmt.Errorf("empty port in %q", raw)
		}
		value, err := strconv.ParseUint(part, 10, 16)
		if err != nil || value == 0 {
			return nil, fmt.Errorf("invalid TCP port %q", part)
		}
		port := uint16(value)
		if _, ok := seen[port]; ok {
			continue
		}
		seen[port] = struct{}{}
		ports = append(ports, port)
	}

	if len(ports) == 0 {
		return nil, fmt.Errorf("no TLS ports configured")
	}
	if len(ports) > maxTLSPorts {
		return nil, fmt.Errorf("got %d ports, maximum is %d", len(ports), maxTLSPorts)
	}
	sort.Slice(ports, func(i, j int) bool { return ports[i] < ports[j] })
	return ports, nil
}

func makeTLSPortSet(ports []uint16) map[uint16]bool {
	set := make(map[uint16]bool, len(ports))
	for _, port := range ports {
		set[port] = true
	}
	return set
}

func setTLSPorts(objects *bpfObjects, ports []uint16) error {
	if objects.TlsPortsMap == nil {
		return fmt.Errorf("tls_ports_map is unavailable")
	}

	iterator := objects.TlsPortsMap.Iterate()
	var key uint16
	var value uint8
	for iterator.Next(&key, &value) {
		if err := objects.TlsPortsMap.Delete(key); err != nil {
			return fmt.Errorf("clear port %d: %w", key, err)
		}
	}
	if err := iterator.Err(); err != nil {
		return fmt.Errorf("iterate configured ports: %w", err)
	}

	for _, port := range ports {
		if err := objects.TlsPortsMap.Put(port, uint8(1)); err != nil {
			return fmt.Errorf("enable port %d: %w", port, err)
		}
	}
	return nil
}
