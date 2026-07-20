package main

import (
	"math/bits"
	"os"
	"runtime"
	"strings"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/perf"
)

/* ===== perf reader: total budget -> per-CPU + fallback ===== */

func openPerfReaderTotalBudget(events *ebpf.Map, totalMB int) (*perf.Reader, int, error) {
	totalBytes := totalMB * 1024 * 1024
	if totalBytes < 256*1024 {
		totalBytes = 256 * 1024
	}

	nCPU := runtime.NumCPU()
	perCPU := totalBytes / nCPU
	if perCPU < 256*1024 {
		perCPU = 256 * 1024
	}

	page := os.Getpagesize()
	pages := perCPU / page
	if pages < 8 {
		pages = 8
	}
	p2 := 1 << (bits.Len(uint(pages)) - 1)
	try := p2 * page

	var rd *perf.Reader
	var err error
	for try >= 256*1024 {
		rd, err = perf.NewReader(events, try)
		if err == nil {
			return rd, try, nil
		}
		if strings.Contains(err.Error(), "cannot allocate memory") || strings.Contains(err.Error(), "can't mmap") {
			try /= 2
			continue
		}
		break
	}
	return nil, 0, err
}
