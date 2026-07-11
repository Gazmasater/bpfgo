package main

import (
	"flag"
	"fmt"
	"os"
	"path/filepath"
	"sort"
	"strings"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/btf"
	"github.com/cilium/ebpf/rlimit"
)

func main() {
	object := flag.String("object", "bpf_x86_bpfel.o", "compiled eBPF ELF object")
	logDir := flag.String("log-dir", "verifier-logs", "directory for verifier logs")
	kernelBTF := flag.String("kernel-btf", "", "optional vmlinux/BTF used for CO-RE relocations")
	flag.Parse()

	if err := rlimit.RemoveMemlock(); err != nil {
		fatalf("remove memlock: %v", err)
	}
	spec, err := ebpf.LoadCollectionSpec(*object)
	if err != nil {
		fatalf("read %s: %v", *object, err)
	}
	if err := os.MkdirAll(*logDir, 0o755); err != nil {
		fatalf("create log directory: %v", err)
	}

	var kernelTypes *btf.Spec
	if *kernelBTF != "" {
		kernelTypes, err = btf.LoadSpec(*kernelBTF)
		if err != nil {
			fatalf("read kernel BTF %s: %v", *kernelBTF, err)
		}
	}
	coll, err := ebpf.NewCollectionWithOptions(spec, ebpf.CollectionOptions{
		Programs: ebpf.ProgramOptions{
			LogLevel:     ebpf.LogLevelBranch | ebpf.LogLevelStats,
			LogSizeStart: 4 * 1024 * 1024,
			KernelTypes:  kernelTypes,
		},
	})
	if err != nil {
		fatalf("verifier rejected collection: %v", err)
	}
	defer coll.Close()

	names := make([]string, 0, len(coll.Programs))
	for name := range coll.Programs {
		names = append(names, name)
	}
	sort.Strings(names)

	for _, name := range names {
		program := coll.Programs[name]
		logText := program.VerifierLog
		path := filepath.Join(*logDir, safeName(name)+".log")
		if err := os.WriteFile(path, []byte(logText), 0o644); err != nil {
			fatalf("write %s: %v", path, err)
		}
		fmt.Printf("PASS %-28s %s\n", name, verifierStats(logText))
	}
	fmt.Printf("verified=%d object=%s kernel_btf=%s logs=%s\n", len(names), *object, valueOrDefault(*kernelBTF, "running-kernel"), *logDir)
}

func verifierStats(logText string) string {
	lines := strings.Split(strings.TrimSpace(logText), "\n")
	for i := len(lines) - 1; i >= 0; i-- {
		if strings.Contains(lines[i], "processed ") || strings.Contains(lines[i], "verification time") {
			return strings.TrimSpace(lines[i])
		}
	}
	return "accepted"
}

func safeName(name string) string {
	return strings.NewReplacer("/", "_", "\\", "_", "..", "_").Replace(name)
}

func valueOrDefault(value, fallback string) string {
	if value == "" {
		return fallback
	}
	return value
}

func fatalf(format string, args ...any) {
	fmt.Fprintf(os.Stderr, "FAIL "+format+"\n", args...)
	os.Exit(1)
}
