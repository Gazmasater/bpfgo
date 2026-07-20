//go:build linux

package main

import (
	"crypto/ecdsa"
	"crypto/elliptic"
	"crypto/rand"
	"crypto/tls"
	"crypto/x509"
	"crypto/x509/pkix"
	"encoding/pem"
	"errors"
	"io"
	"math/big"
	"net"
	"os"
	"os/exec"
	"path/filepath"
	"strconv"
	"strings"
	"sync"
	"syscall"
	"testing"
	"time"
	"unsafe"

	"github.com/cilium/ebpf/rlimit"
)

const integrationEnv = "BPFGO_INTEGRATION"

const (
	pidFilterHelperEnv      = "BPFGO_PID_FILTER_HELPER"
	pidFilterAddressEnv     = "BPFGO_PID_FILTER_ADDRESS"
	pidFilterStartFileEnv   = "BPFGO_PID_FILTER_START_FILE"
	pidFilterHelperTestName = "^TestIntegrationPIDFilterClientHelper$"
)

func TestIntegrationCapturesUDPSyscalls(t *testing.T) {
	if os.Getenv(integrationEnv) != "1" {
		t.Skipf("set %s=1 to run privileged BPF integration tests", integrationEnv)
	}
	if os.Geteuid() != 0 {
		t.Skip("BPF integration test requires root or equivalent BPF capabilities")
	}

	if err := rlimit.RemoveMemlock(); err != nil {
		t.Fatalf("remove memlock: %v", err)
	}

	var objects bpfObjects
	if err := loadBpfObjects(&objects, nil); err != nil {
		t.Fatalf("load BPF objects: %v", err)
	}
	t.Cleanup(func() { _ = objects.Close() })

	links, err := attachProbeGroups(buildProbeGroups(&objects, false, false, false))
	if err != nil {
		t.Fatalf("attach core probes: %v", err)
	}
	t.Cleanup(func() { closeLinks(links) })
	if len(links) != 17 {
		t.Fatalf("attached %d core probes, want 17", len(links))
	}

	reader, _, err := openPerfReaderTotalBudget(objects.TraceEvents, 2)
	if err != nil {
		t.Fatalf("open perf reader: %v", err)
	}
	t.Cleanup(func() { _ = reader.Close() })

	serverFD, err := syscall.Socket(syscall.AF_INET, syscall.SOCK_DGRAM, 0)
	if err != nil {
		t.Fatalf("create UDP server socket: %v", err)
	}
	t.Cleanup(func() { _ = syscall.Close(serverFD) })
	loopback := [4]byte{127, 0, 0, 1}
	if err := syscall.Bind(serverFD, &syscall.SockaddrInet4{Addr: loopback}); err != nil {
		t.Fatalf("bind UDP server socket: %v", err)
	}
	serverAddr, err := syscall.Getsockname(serverFD)
	if err != nil {
		t.Fatalf("get UDP server address: %v", err)
	}
	serverPort := serverAddr.(*syscall.SockaddrInet4).Port

	clientFD, err := syscall.Socket(syscall.AF_INET, syscall.SOCK_DGRAM, 0)
	if err != nil {
		t.Fatalf("create UDP client socket: %v", err)
	}
	t.Cleanup(func() { _ = syscall.Close(clientFD) })
	payload := []byte("bpfgo-integration")
	if err := syscall.Sendto(clientFD, payload, 0, &syscall.SockaddrInet4{Addr: loopback, Port: serverPort}); err != nil {
		t.Fatalf("send UDP datagram: %v", err)
	}

	buf := make([]byte, 64)
	if _, _, err := syscall.Recvfrom(serverFD, buf, 0); err != nil {
		t.Fatalf("receive UDP datagram: %v", err)
	}

	deadline := time.Now().Add(3 * time.Second)
	seen := map[uint8]bool{}
	for !seen[EV_SENDTO] || !seen[EV_RECVFROM] {
		reader.SetDeadline(deadline)
		record, err := reader.Read()
		if err != nil {
			if errors.Is(err, os.ErrDeadlineExceeded) {
				break
			}
			t.Fatalf("read perf record: %v", err)
		}
		if record.LostSamples != 0 {
			t.Fatalf("lost %d perf samples during integration test", record.LostSamples)
		}
		if len(record.RawSample) < int(unsafe.Sizeof(bpfTraceInfo{})) {
			continue
		}
		event := *(*bpfTraceInfo)(unsafe.Pointer(&record.RawSample[0]))
		if event.Tgid != uint32(os.Getpid()) {
			continue
		}
		if event.Event == EV_SENDTO || event.Event == EV_RECVFROM {
			seen[event.Event] = true
		}
	}

	if !seen[EV_SENDTO] || !seen[EV_RECVFROM] {
		t.Fatalf("missing UDP events: sendto=%t recvfrom=%t", seen[EV_SENDTO], seen[EV_RECVFROM])
	}
	if got := string(buf[:len(payload)]); got != string(payload) {
		t.Fatalf("received payload %q, want %q", got, payload)
	}
	if serverPort == 0 {
		t.Fatal("kernel assigned an invalid UDP port")
	}
}

func TestIntegrationCapturesTCPLoopbackSyscalls(t *testing.T) {
	requireIntegration(t)

	var objects bpfObjects
	if err := loadBpfObjects(&objects, nil); err != nil {
		t.Fatalf("load BPF objects: %v", err)
	}
	t.Cleanup(func() { _ = objects.Close() })
	links, err := attachProbeGroups(buildProbeGroups(&objects, false, true, false))
	if err != nil {
		t.Fatalf("attach core and read/write probes: %v", err)
	}
	t.Cleanup(func() { closeLinks(links) })
	if len(links) != 21 {
		t.Fatalf("attached %d probes, want 21", len(links))
	}

	reader, _, err := openPerfReaderTotalBudget(objects.TraceEvents, 2)
	if err != nil {
		t.Fatalf("open perf reader: %v", err)
	}
	t.Cleanup(func() { _ = reader.Close() })

	listener, err := net.Listen("tcp4", "127.0.0.1:0")
	if err != nil {
		t.Fatalf("listen on TCP loopback: %v", err)
	}
	t.Cleanup(func() { _ = listener.Close() })

	serverErr := make(chan error, 1)
	go func() {
		conn, err := listener.Accept()
		if err == nil {
			defer conn.Close()
			buf := make([]byte, len("bpfgo-tcp"))
			_, err = conn.Read(buf)
			if err == nil {
				_, err = conn.Write([]byte("ok"))
			}
		}
		serverErr <- err
	}()

	client, err := net.Dial("tcp4", listener.Addr().String())
	if err != nil {
		t.Fatalf("dial TCP loopback: %v", err)
	}
	if _, err := client.Write([]byte("bpfgo-tcp")); err != nil {
		_ = client.Close()
		t.Fatalf("write TCP payload: %v", err)
	}
	response := make([]byte, 2)
	if _, err := client.Read(response); err != nil {
		_ = client.Close()
		t.Fatalf("read TCP response: %v", err)
	}
	if err := client.Close(); err != nil {
		t.Fatalf("close TCP client: %v", err)
	}
	if err := <-serverErr; err != nil {
		t.Fatalf("serve TCP loopback: %v", err)
	}
	if got := string(response); got != "ok" {
		t.Fatalf("TCP response %q, want %q", got, "ok")
	}

	deadline := time.Now().Add(3 * time.Second)
	seen := map[uint8]bool{}
	for !seen[EV_CONNECT] || !seen[EV_ACCEPT] || !seen[EV_WRITE] || !seen[EV_READ] {
		reader.SetDeadline(deadline)
		record, err := reader.Read()
		if err != nil {
			if errors.Is(err, os.ErrDeadlineExceeded) {
				break
			}
			t.Fatalf("read perf record: %v", err)
		}
		if record.LostSamples != 0 {
			t.Fatalf("lost %d perf samples during integration test", record.LostSamples)
		}
		if len(record.RawSample) < int(unsafe.Sizeof(bpfTraceInfo{})) {
			continue
		}
		event := *(*bpfTraceInfo)(unsafe.Pointer(&record.RawSample[0]))
		if event.Tgid != uint32(os.Getpid()) {
			continue
		}
		switch event.Event {
		case EV_CONNECT, EV_ACCEPT, EV_WRITE, EV_READ:
			seen[event.Event] = true
		}
	}

	for _, event := range []uint8{EV_CONNECT, EV_ACCEPT, EV_WRITE, EV_READ} {
		if !seen[event] {
			t.Fatalf("missing TCP loopback event %d; seen=%v", event, seen)
		}
	}
}

func TestIntegrationExtractsTLSSNI(t *testing.T) {
	requireIntegration(t)

	var objects bpfObjects
	if err := loadBpfObjects(&objects, nil); err != nil {
		t.Fatalf("load BPF objects: %v", err)
	}
	t.Cleanup(func() { _ = objects.Close() })
	if err := setTLSCaptureEnabled(&objects, true); err != nil {
		t.Fatalf("enable TLS capture: %v", err)
	}

	links, err := attachProbeGroups(buildProbeGroups(&objects, false, true, false))
	if err != nil {
		t.Fatalf("attach core and read/write probes: %v", err)
	}
	t.Cleanup(func() { closeLinks(links) })

	reader, _, err := openPerfReaderTotalBudget(objects.TlsEvents, 2)
	if err != nil {
		t.Fatalf("open TLS perf reader: %v", err)
	}
	t.Cleanup(func() { _ = reader.Close() })

	serverConfig, err := testTLSConfig()
	if err != nil {
		t.Fatalf("create TLS server config: %v", err)
	}
	listener, err := tls.Listen("tcp4", "127.0.0.1:443", serverConfig)
	if errors.Is(err, syscall.EADDRINUSE) {
		t.Skip("TCP port 443 is already in use; TLS SNI probe requires it")
	}
	if err != nil {
		t.Fatalf("listen for TLS on port 443: %v", err)
	}
	t.Cleanup(func() { _ = listener.Close() })

	serverErr := make(chan error, 1)
	go func() {
		conn, err := listener.Accept()
		if err == nil {
			defer conn.Close()
			err = conn.(*tls.Conn).Handshake()
		}
		serverErr <- err
	}()

	const serverName = "bpfgo.test"
	client, err := tls.Dial("tcp4", "127.0.0.1:443", &tls.Config{
		ServerName:         serverName,
		InsecureSkipVerify: true, // test listener uses a temporary self-signed certificate.
		MinVersion:         tls.VersionTLS12,
		MaxVersion:         tls.VersionTLS12,
		CurvePreferences:   []tls.CurveID{tls.CurveP256},
	})
	if err != nil {
		t.Fatalf("TLS client handshake: %v", err)
	}
	if err := client.Close(); err != nil {
		t.Fatalf("close TLS client: %v", err)
	}
	if err := <-serverErr; err != nil {
		t.Fatalf("TLS server handshake: %v", err)
	}

	assembler := newTLSAssembler(time.Minute, 16)
	deadline := time.Now().Add(3 * time.Second)
	for {
		reader.SetDeadline(deadline)
		record, err := reader.Read()
		if err != nil {
			if errors.Is(err, os.ErrDeadlineExceeded) {
				break
			}
			t.Fatalf("read TLS perf record: %v", err)
		}
		if record.LostSamples != 0 {
			t.Fatalf("lost %d TLS perf samples during integration test", record.LostSamples)
		}
		if len(record.RawSample) < int(unsafe.Sizeof(bpfTlsChunkEvent{})) {
			continue
		}
		event := *(*bpfTlsChunkEvent)(unsafe.Pointer(&record.RawSample[0]))
		if event.Tgid != uint32(os.Getpid()) || event.Event != EV_TLS_CHUNK || event.Len > uint32(len(event.Data)) {
			continue
		}
		if sni, ok := assembler.Push(event.Cookie, event.Data[:event.Len]); ok && sni == serverName {
			return
		}
	}
	t.Fatalf("did not extract SNI %q from TLS ClientHello", serverName)
}

func TestIntegrationTLSCaptureDisabled(t *testing.T) {
	requireIntegration(t)

	var objects bpfObjects
	if err := loadBpfObjects(&objects, nil); err != nil {
		t.Fatalf("load BPF objects: %v", err)
	}
	t.Cleanup(func() { _ = objects.Close() })
	if err := setTLSCaptureEnabled(&objects, false); err != nil {
		t.Fatalf("disable TLS capture: %v", err)
	}

	links, err := attachProbeGroups(buildProbeGroups(&objects, false, true, false))
	if err != nil {
		t.Fatalf("attach core and read/write probes: %v", err)
	}
	t.Cleanup(func() { closeLinks(links) })

	reader, _, err := openPerfReaderTotalBudget(objects.TlsEvents, 2)
	if err != nil {
		t.Fatalf("open TLS perf reader: %v", err)
	}
	t.Cleanup(func() { _ = reader.Close() })

	listener, err := net.Listen("tcp4", "127.0.0.1:443")
	if errors.Is(err, syscall.EADDRINUSE) {
		t.Skip("TCP port 443 is already in use; TLS capture gate test requires it")
	}
	if err != nil {
		t.Fatalf("listen on TCP loopback: %v", err)
	}
	t.Cleanup(func() { _ = listener.Close() })

	serverErr := make(chan error, 1)
	go func() {
		conn, err := listener.Accept()
		if err == nil {
			defer conn.Close()
			buf := make([]byte, len("bpfgo-gate"))
			_, err = io.ReadFull(conn, buf)
		}
		serverErr <- err
	}()

	client, err := net.Dial("tcp4", listener.Addr().String())
	if err != nil {
		t.Fatalf("dial capture-gate server: %v", err)
	}
	if _, err := client.Write([]byte("bpfgo-gate")); err != nil {
		_ = client.Close()
		t.Fatalf("write capture-gate payload: %v", err)
	}
	if err := client.Close(); err != nil {
		t.Fatalf("close capture-gate client: %v", err)
	}
	if err := <-serverErr; err != nil {
		t.Fatalf("serve capture-gate client: %v", err)
	}

	reader.SetDeadline(time.Now().Add(500 * time.Millisecond))
	_, err = reader.Read()
	if !errors.Is(err, os.ErrDeadlineExceeded) {
		t.Fatalf("TLS capture emitted an event while disabled: %v", err)
	}
}

func TestIntegrationCorrelatesTLSSNIWithFlow(t *testing.T) {
	requireIntegration(t)

	tracer := startIntegrationTracerForPID(t, os.Getpid())
	const serverName = "bpfgo.test"

	serverConfig, err := testTLSConfig()
	if err != nil {
		t.Fatalf("create TLS server config: %v", err)
	}
	listener, err := tls.Listen("tcp4", "127.0.0.1:443", serverConfig)
	if errors.Is(err, syscall.EADDRINUSE) {
		t.Skip("TCP port 443 is already in use; TLS SNI probe requires it")
	}
	if err != nil {
		t.Fatalf("listen for TLS on port 443: %v", err)
	}
	t.Cleanup(func() { _ = listener.Close() })

	serverErr := make(chan error, 1)
	go func() {
		conn, err := listener.Accept()
		if err == nil {
			defer conn.Close()
			if err = conn.(*tls.Conn).Handshake(); err == nil {
				_, err = io.Copy(io.Discard, conn)
				if errors.Is(err, io.EOF) {
					err = nil
				}
			}
		}
		serverErr <- err
	}()

	client, err := tls.Dial("tcp4", "127.0.0.1:443", testTLSClientConfig(serverName))
	if err != nil {
		t.Fatalf("TLS client handshake: %v", err)
	}

	// Let the separate tracer process consume the TLS chunk and update the live flow
	// before the close syscall asks it to print the final flow summary.
	time.Sleep(300 * time.Millisecond)
	if err := client.Close(); err != nil {
		t.Fatalf("close TLS client: %v", err)
	}
	if err := <-serverErr; err != nil {
		t.Fatalf("TLS server handshake: %v", err)
	}
	time.Sleep(300 * time.Millisecond)

	output := tracer.stop(t)
	if !strings.Contains(output, "FLOW  TCP") || !strings.Contains(output, "end=close() sni="+serverName) {
		t.Fatalf("tracer did not correlate SNI with the closed flow:\n%s", output)
	}
}

func TestIntegrationDisablesTLSWhenRWOff(t *testing.T) {
	requireIntegration(t)

	tracer := startIntegrationTracerForPID(t, os.Getpid(), "-rw=false")
	serverConfig, err := testTLSConfig()
	if err != nil {
		t.Fatalf("create TLS server config: %v", err)
	}
	listener, err := tls.Listen("tcp4", "127.0.0.1:443", serverConfig)
	if errors.Is(err, syscall.EADDRINUSE) {
		t.Skip("TCP port 443 is already in use; TLS SNI probe requires it")
	}
	if err != nil {
		t.Fatalf("listen for TLS on port 443: %v", err)
	}
	t.Cleanup(func() { _ = listener.Close() })

	serverErr := make(chan error, 1)
	go func() {
		conn, err := listener.Accept()
		if err == nil {
			defer conn.Close()
			if err = conn.(*tls.Conn).Handshake(); err == nil {
				_, err = io.Copy(io.Discard, conn)
				if errors.Is(err, io.EOF) {
					err = nil
				}
			}
		}
		serverErr <- err
	}()

	client, err := tls.Dial("tcp4", "127.0.0.1:443", testTLSClientConfig("bpfgo.test"))
	if err != nil {
		t.Fatalf("TLS client handshake: %v", err)
	}
	if err := client.Close(); err != nil {
		t.Fatalf("close TLS client: %v", err)
	}
	if err := <-serverErr; err != nil {
		t.Fatalf("TLS server handshake: %v", err)
	}
	time.Sleep(300 * time.Millisecond)

	output := tracer.stop(t)
	if !strings.Contains(output, "FLOW  TCP") {
		t.Fatalf("core probes missed the TLS TCP flow with -rw=false:\n%s", output)
	}
	if strings.Contains(output, "sni=") {
		t.Fatalf("TLS SNI was captured even though -rw=false:\n%s", output)
	}
}

func TestIntegrationDisablesSNIParsing(t *testing.T) {
	requireIntegration(t)

	tracer := startIntegrationTracerForPID(t, os.Getpid(), "-sni=false")
	serverConfig, err := testTLSConfig()
	if err != nil {
		t.Fatalf("create TLS server config: %v", err)
	}
	listener, err := tls.Listen("tcp4", "127.0.0.1:443", serverConfig)
	if errors.Is(err, syscall.EADDRINUSE) {
		t.Skip("TCP port 443 is already in use; TLS SNI probe requires it")
	}
	if err != nil {
		t.Fatalf("listen for TLS on port 443: %v", err)
	}
	t.Cleanup(func() { _ = listener.Close() })

	serverErr := make(chan error, 1)
	go func() {
		conn, err := listener.Accept()
		if err == nil {
			defer conn.Close()
			if err = conn.(*tls.Conn).Handshake(); err == nil {
				_, err = io.Copy(io.Discard, conn)
				if errors.Is(err, io.EOF) {
					err = nil
				}
			}
		}
		serverErr <- err
	}()

	client, err := tls.Dial("tcp4", "127.0.0.1:443", testTLSClientConfig("bpfgo.test"))
	if err != nil {
		t.Fatalf("TLS client handshake: %v", err)
	}
	time.Sleep(300 * time.Millisecond)
	if err := client.Close(); err != nil {
		t.Fatalf("close TLS client: %v", err)
	}
	if err := <-serverErr; err != nil {
		t.Fatalf("TLS server handshake: %v", err)
	}
	time.Sleep(300 * time.Millisecond)

	output := tracer.stop(t)
	if !strings.Contains(output, "SNI=false") || !strings.Contains(output, "FLOW  TCP") {
		t.Fatalf("tracer did not run a TCP flow with SNI parsing disabled:\n%s", output)
	}
	if strings.Contains(output, "sni=") {
		t.Fatalf("SNI was added to flow despite -sni=false:\n%s", output)
	}
}

func TestIntegrationDisablesL3Hints(t *testing.T) {
	requireIntegration(t)

	tracer := startIntegrationTracerForPID(t, os.Getpid())
	listener, err := net.Listen("tcp4", "127.0.0.1:0")
	if err != nil {
		t.Fatalf("listen on TCP loopback: %v", err)
	}
	t.Cleanup(func() { _ = listener.Close() })

	serverErr := make(chan error, 1)
	go func() {
		conn, err := listener.Accept()
		if err == nil {
			defer conn.Close()
			buf := make([]byte, len("l3-off"))
			_, err = io.ReadFull(conn, buf)
			if err == nil {
				_, err = conn.Write([]byte("ok"))
			}
		}
		serverErr <- err
	}()

	client, err := net.Dial("tcp4", listener.Addr().String())
	if err != nil {
		t.Fatalf("dial TCP loopback: %v", err)
	}
	if _, err := client.Write([]byte("l3-off")); err != nil {
		_ = client.Close()
		t.Fatalf("write TCP payload: %v", err)
	}
	response := make([]byte, 2)
	if _, err := io.ReadFull(client, response); err != nil {
		_ = client.Close()
		t.Fatalf("read TCP response: %v", err)
	}
	if err := client.Close(); err != nil {
		t.Fatalf("close TCP client: %v", err)
	}
	if err := <-serverErr; err != nil {
		t.Fatalf("serve TCP loopback: %v", err)
	}
	if got := string(response); got != "ok" {
		t.Fatalf("TCP response %q, want %q", got, "ok")
	}
	time.Sleep(300 * time.Millisecond)

	output := tracer.stop(t)
	if !strings.Contains(output, "FLOW  TCP") {
		t.Fatalf("core probes missed TCP flow with -l3=false:\n%s", output)
	}
	if !strings.Contains(output, `probe group enabled: name="core socket syscalls" tracepoints=17`) ||
		!strings.Contains(output, `probe group enabled: name="socket read/write syscalls" tracepoints=4`) {
		t.Fatalf("tracer did not report expected enabled groups:\n%s", output)
	}
	if strings.Contains(output, `name="L3 socket hints"`) || strings.Contains(output, "net_dev_queue") {
		t.Fatalf("L3 group was attached despite -l3=false:\n%s", output)
	}
}

func TestIntegrationDisablesMmsg(t *testing.T) {
	requireIntegration(t)

	tracer := startIntegrationTracerForPID(t, os.Getpid(), "-mmsg=false")
	serverFD, err := syscall.Socket(syscall.AF_INET, syscall.SOCK_DGRAM, 0)
	if err != nil {
		t.Fatalf("create UDP server socket: %v", err)
	}
	t.Cleanup(func() {
		if serverFD >= 0 {
			_ = syscall.Close(serverFD)
		}
	})
	loopback := [4]byte{127, 0, 0, 1}
	if err := syscall.Bind(serverFD, &syscall.SockaddrInet4{Addr: loopback}); err != nil {
		t.Fatalf("bind UDP server socket: %v", err)
	}
	serverAddr, err := syscall.Getsockname(serverFD)
	if err != nil {
		t.Fatalf("get UDP server address: %v", err)
	}
	serverPort := serverAddr.(*syscall.SockaddrInet4).Port

	clientFD, err := syscall.Socket(syscall.AF_INET, syscall.SOCK_DGRAM, 0)
	if err != nil {
		t.Fatalf("create UDP client socket: %v", err)
	}
	t.Cleanup(func() {
		if clientFD >= 0 {
			_ = syscall.Close(clientFD)
		}
	})
	payload := []byte("mmsg-off")
	if err := syscall.Sendto(clientFD, payload, 0, &syscall.SockaddrInet4{Addr: loopback, Port: serverPort}); err != nil {
		t.Fatalf("send UDP datagram: %v", err)
	}
	received := make([]byte, len(payload))
	if _, _, err := syscall.Recvfrom(serverFD, received, 0); err != nil {
		t.Fatalf("receive UDP datagram: %v", err)
	}
	if got := string(received); got != string(payload) {
		t.Fatalf("received UDP payload %q, want %q", got, payload)
	}
	if err := syscall.Close(clientFD); err != nil {
		t.Fatalf("close UDP client socket: %v", err)
	}
	clientFD = -1
	if err := syscall.Close(serverFD); err != nil {
		t.Fatalf("close UDP server socket: %v", err)
	}
	serverFD = -1
	time.Sleep(300 * time.Millisecond)

	output := tracer.stop(t)
	if !strings.Contains(output, "FLOW  UDP") {
		t.Fatalf("core UDP probes missed flow with -mmsg=false:\n%s", output)
	}
	if !strings.Contains(output, `probe group enabled: name="core socket syscalls" tracepoints=17`) {
		t.Fatalf("tracer did not report the core group:\n%s", output)
	}
	if strings.Contains(output, `name="mmsg syscalls"`) {
		t.Fatalf("mmsg group was attached despite -mmsg=false:\n%s", output)
	}
}

func TestIntegrationFiltersOtherPIDs(t *testing.T) {
	requireIntegration(t)

	listener, err := net.Listen("tcp4", "127.0.0.1:0")
	if err != nil {
		t.Fatalf("listen on TCP loopback: %v", err)
	}
	t.Cleanup(func() { _ = listener.Close() })

	serverErr := make(chan error, 1)
	go func() {
		conn, err := listener.Accept()
		if err == nil {
			defer conn.Close()
			buf := make([]byte, len("pid-filter"))
			_, err = io.ReadFull(conn, buf)
			if err == nil {
				_, err = conn.Write([]byte("ok"))
			}
		}
		serverErr <- err
	}()

	startFile := filepath.Join(t.TempDir(), "start")
	helper := exec.Command(os.Args[0], "-test.run="+pidFilterHelperTestName)
	helper.Env = append(os.Environ(),
		pidFilterHelperEnv+"=1",
		pidFilterAddressEnv+"="+listener.Addr().String(),
		pidFilterStartFileEnv+"="+startFile,
	)
	helperOutput := &strings.Builder{}
	helper.Stdout = helperOutput
	helper.Stderr = helperOutput
	if err := helper.Start(); err != nil {
		t.Fatalf("start PID-filter client helper: %v", err)
	}
	helperDone := false
	t.Cleanup(func() {
		if !helperDone && helper.Process != nil {
			_ = helper.Process.Kill()
			_ = helper.Wait()
		}
	})

	tracer := startIntegrationTracerForPID(t, helper.Process.Pid)
	if err := os.WriteFile(startFile, []byte("go"), 0o600); err != nil {
		t.Fatalf("release PID-filter client helper: %v", err)
	}
	if err := helper.Wait(); err != nil {
		t.Fatalf("PID-filter client helper: %v\n%s", err, helperOutput.String())
	}
	helperDone = true
	if err := <-serverErr; err != nil {
		t.Fatalf("serve PID-filter client: %v", err)
	}
	time.Sleep(300 * time.Millisecond)

	output := tracer.stop(t)
	helperPID := strconv.Itoa(helper.Process.Pid)
	parentPID := strconv.Itoa(os.Getpid())
	if !strings.Contains(output, "FLOW  TCP   pid="+helperPID+"(") {
		t.Fatalf("tracer missed flow from selected PID %s:\n%s", helperPID, output)
	}
	if strings.Contains(output, "pid="+parentPID+"(") {
		t.Fatalf("tracer emitted flow from unselected parent PID %s:\n%s", parentPID, output)
	}
}

func TestIntegrationPIDFilterClientHelper(t *testing.T) {
	if os.Getenv(pidFilterHelperEnv) != "1" {
		t.Skip("helper process for TestIntegrationFiltersOtherPIDs")
	}
	address := os.Getenv(pidFilterAddressEnv)
	startFile := os.Getenv(pidFilterStartFileEnv)
	if address == "" || startFile == "" {
		t.Fatal("PID-filter helper is missing its address or start file")
	}

	deadline := time.Now().Add(10 * time.Second)
	for {
		if _, err := os.Stat(startFile); err == nil {
			break
		} else if !errors.Is(err, os.ErrNotExist) {
			t.Fatalf("check helper start file: %v", err)
		}
		if time.Now().After(deadline) {
			t.Fatal("timed out waiting for tracer startup")
		}
		time.Sleep(20 * time.Millisecond)
	}

	conn, err := net.DialTimeout("tcp4", address, 3*time.Second)
	if err != nil {
		t.Fatalf("dial PID-filter server: %v", err)
	}
	defer conn.Close()
	if _, err := conn.Write([]byte("pid-filter")); err != nil {
		t.Fatalf("write PID-filter payload: %v", err)
	}
	response := make([]byte, 2)
	if _, err := io.ReadFull(conn, response); err != nil {
		t.Fatalf("read PID-filter response: %v", err)
	}
	if got := string(response); got != "ok" {
		t.Fatalf("PID-filter response %q, want %q", got, "ok")
	}
}

type integrationTracer struct {
	cmd     *exec.Cmd
	waitCh  <-chan error
	lines   *lockedLines
	stopMu  sync.Mutex
	stopped bool
}

type lockedLines struct {
	sync.Mutex
	strings.Builder
}

func (l *lockedLines) add(line string) {
	l.Lock()
	l.WriteString(line)
	l.WriteByte('\n')
	l.Unlock()
}

func (l *lockedLines) String() string {
	l.Lock()
	defer l.Unlock()
	return l.Builder.String()
}

func startIntegrationTracerForPID(t *testing.T, pid int, options ...string) *integrationTracer {
	t.Helper()
	binary := filepath.Join(t.TempDir(), "bpfgo-e2e")
	build := exec.Command("go", "build", "-o", binary, ".")
	if output, err := build.CombinedOutput(); err != nil {
		t.Fatalf("build tracer binary: %v\n%s", err, output)
	}

	args := []string{
		"-pid=" + strconv.Itoa(pid),
		"-pprof=false",
		"-resolve=false",
		"-hostsPrefill=false",
		"-mmsg=false",
		"-l3=false",
		"-rw=true",
		"-sni=true",
		"-perfMB=1",
	}
	args = append(args, options...)
	cmd := exec.Command(binary, args...)
	stdout, err := cmd.StdoutPipe()
	if err != nil {
		t.Fatalf("open tracer stdout: %v", err)
	}
	stderr, err := cmd.StderrPipe()
	if err != nil {
		t.Fatalf("open tracer stderr: %v", err)
	}

	lines := &lockedLines{}
	ready := make(chan struct{})
	var readyOnce sync.Once
	var readers sync.WaitGroup
	consume := func(r io.Reader) {
		defer readers.Done()
		buf := make([]byte, 4096)
		for {
			n, err := r.Read(buf)
			if n > 0 {
				chunk := string(buf[:n])
				lines.add(chunk)
				if strings.Contains(chunk, "flow summaries") {
					readyOnce.Do(func() { close(ready) })
				}
			}
			if err != nil {
				return
			}
		}
	}

	readers.Add(2)
	if err := cmd.Start(); err != nil {
		t.Fatalf("start tracer: %v", err)
	}
	go consume(stdout)
	go consume(stderr)
	waitCh := make(chan error, 1)
	go func() {
		err := cmd.Wait()
		readers.Wait()
		waitCh <- err
	}()

	tracer := &integrationTracer{cmd: cmd, waitCh: waitCh, lines: lines}
	t.Cleanup(func() {
		tracer.stopSilently()
	})
	select {
	case <-ready:
		return tracer
	case err := <-waitCh:
		t.Fatalf("tracer exited before startup: %v\n%s", err, lines.String())
	case <-time.After(10 * time.Second):
		tracer.stopSilently()
		t.Fatalf("tracer did not become ready:\n%s", lines.String())
	}
	return nil
}

func (t *integrationTracer) stop(tb testing.TB) string {
	tb.Helper()
	t.stopMu.Lock()
	if !t.stopped {
		t.stopped = true
		_ = t.cmd.Process.Signal(syscall.SIGTERM)
	}
	t.stopMu.Unlock()

	select {
	case err := <-t.waitCh:
		if err != nil {
			tb.Fatalf("stop tracer: %v\n%s", err, t.lines.String())
		}
	case <-time.After(10 * time.Second):
		tb.Fatalf("tracer did not stop:\n%s", t.lines.String())
	}
	return t.lines.String()
}

func (t *integrationTracer) stopSilently() {
	t.stopMu.Lock()
	if t.stopped {
		t.stopMu.Unlock()
		return
	}
	t.stopped = true
	_ = t.cmd.Process.Signal(syscall.SIGTERM)
	t.stopMu.Unlock()
	select {
	case <-t.waitCh:
	case <-time.After(10 * time.Second):
		_ = t.cmd.Process.Kill()
	}
}

func requireIntegration(t *testing.T) {
	t.Helper()
	if os.Getenv(integrationEnv) != "1" {
		t.Skipf("set %s=1 to run privileged BPF integration tests", integrationEnv)
	}
	if os.Geteuid() != 0 {
		t.Skip("BPF integration test requires root or equivalent BPF capabilities")
	}
	if err := rlimit.RemoveMemlock(); err != nil {
		t.Fatalf("remove memlock: %v", err)
	}
}

func testTLSConfig() (*tls.Config, error) {
	key, err := ecdsa.GenerateKey(elliptic.P256(), rand.Reader)
	if err != nil {
		return nil, err
	}
	certificate := x509.Certificate{
		SerialNumber: big.NewInt(1),
		Subject:      pkix.Name{CommonName: "bpfgo.test"},
		DNSNames:     []string{"bpfgo.test"},
		NotBefore:    time.Now().Add(-time.Minute),
		NotAfter:     time.Now().Add(time.Hour),
		KeyUsage:     x509.KeyUsageDigitalSignature,
		ExtKeyUsage:  []x509.ExtKeyUsage{x509.ExtKeyUsageServerAuth},
	}
	certificateDER, err := x509.CreateCertificate(rand.Reader, &certificate, &certificate, &key.PublicKey, key)
	if err != nil {
		return nil, err
	}
	certificatePEM, err := x509.MarshalPKCS8PrivateKey(key)
	if err != nil {
		return nil, err
	}
	pair, err := tls.X509KeyPair(
		pem.EncodeToMemory(&pem.Block{Type: "CERTIFICATE", Bytes: certificateDER}),
		pem.EncodeToMemory(&pem.Block{Type: "PRIVATE KEY", Bytes: certificatePEM}),
	)
	if err != nil {
		return nil, err
	}
	return &tls.Config{
		Certificates: []tls.Certificate{pair},
		MinVersion:   tls.VersionTLS12,
		MaxVersion:   tls.VersionTLS12,
	}, nil
}

func testTLSClientConfig(serverName string) *tls.Config {
	return &tls.Config{
		ServerName:         serverName,
		InsecureSkipVerify: true, // test listener uses a temporary self-signed certificate.
		MinVersion:         tls.VersionTLS12,
		MaxVersion:         tls.VersionTLS12,
		CurvePreferences:   []tls.CurveID{tls.CurveP256},
	}
}
