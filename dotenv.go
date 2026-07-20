package main

import (
	"bufio"
	"flag"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"strings"
)

/* ===== .env loader (no deps) ===== */

func fileExists(path string) bool {
	st, err := os.Stat(path)
	return err == nil && !st.IsDir()
}

func loadDotEnvFile(path string, override bool) error {
	f, err := os.Open(path)
	if err != nil {
		return err
	}
	defer f.Close()
	return loadDotEnvReader(f, override)
}

func loadDotEnvReader(r io.Reader, override bool) error {
	sc := bufio.NewScanner(r)
	for sc.Scan() {
		line := strings.TrimSpace(sc.Text())
		if line == "" || strings.HasPrefix(line, "#") {
			continue
		}
		if i := strings.Index(line, " #"); i >= 0 {
			line = strings.TrimSpace(line[:i])
		}
		if strings.HasPrefix(line, "export ") {
			line = strings.TrimSpace(strings.TrimPrefix(line, "export "))
		}
		i := strings.IndexByte(line, '=')
		if i <= 0 {
			continue
		}
		key := strings.TrimSpace(line[:i])
		val := strings.TrimSpace(line[i+1:])
		if len(val) >= 2 {
			if (val[0] == '"' && val[len(val)-1] == '"') || (val[0] == '\'' && val[len(val)-1] == '\'') {
				val = val[1 : len(val)-1]
			}
		}
		if key == "" {
			continue
		}
		if !override {
			if _, exists := os.LookupEnv(key); exists {
				continue
			}
		}
		_ = os.Setenv(key, val)
	}
	return sc.Err()
}

func loadDotEnvAuto() (string, error) {
	if p, ok := os.LookupEnv("BPFGO_DOTENV"); ok && strings.TrimSpace(p) != "" {
		p = strings.TrimSpace(p)
		if fileExists(p) {
			return p, loadDotEnvFile(p, false)
		}
		return p, fmt.Errorf("BPFGO_DOTENV set but file not found: %s", p)
	}
	if fileExists(".env") {
		return ".env", loadDotEnvFile(".env", false)
	}
	if exe, err := os.Executable(); err == nil {
		dir := filepath.Dir(exe)
		p := filepath.Join(dir, ".env")
		if fileExists(p) {
			return p, loadDotEnvFile(p, false)
		}
	}
	return "", nil
}

func applyEnvToFlags() {
	set := func(flagName, envName string) {
		if v, ok := os.LookupEnv(envName); ok && strings.TrimSpace(v) != "" {
			_ = flag.Set(flagName, strings.TrimSpace(v))
		}
	}

	set("perfMB", "BPFGO_PERF_MB")
	set("pprof", "BPFGO_PPROF")
	set("pprofAddr", "BPFGO_PPROF_ADDR")
	set("ttl", "BPFGO_TTL")
	set("print", "BPFGO_SWEEP")
	set("pid", "BPFGO_ONLY_PID")
	set("comm", "BPFGO_ONLY_COMM")
	set("rw", "BPFGO_RW")
	set("mmsg", "BPFGO_MMSG")

	set("resolve", "BPFGO_RESOLVE")
	set("resolveTTL", "BPFGO_RESOLVE_TTL")
	set("resolveNegTTL", "BPFGO_RESOLVE_NEG_TTL")
	set("resolveWorkers", "BPFGO_RESOLVE_WORKERS")
	set("resolveTimeout", "BPFGO_RESOLVE_TIMEOUT")
	set("resolveQ", "BPFGO_RESOLVE_Q")
	set("hostState", "BPFGO_HOST_STATE")

	set("hostsPrefill", "BPFGO_HOSTS_PREFILL")
	set("hostsFile", "BPFGO_HOSTS_FILE")
	set("hostsTTL", "BPFGO_HOSTS_TTL")

	set("resolveSweepEach", "BPFGO_RESOLVE_SWEEP_EACH")
	set("resolvePokeEach", "BPFGO_RESOLVE_POKE_EACH")

	set("l3ttl", "BPFGO_L3_TTL")
	set("openDelay", "BPFGO_OPEN_DELAY")
	set("l3SweepEach", "BPFGO_L3_SWEEP_EACH")

	set("sni", "BPFGO_SNI")
	set("sniTTL", "BPFGO_SNI_TTL")
	set("sniMax", "BPFGO_SNI_MAX")
}
