
https://platform-balun.ru/teach/control/stream/view/id/930914027
user: mat3x@mail.ru
pass: Vkalashnikov00f!



bpf2go -output-dir . -tags linux -type trace_info -go-package=main -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)



sudo nft add table ip6 test
sudo nft add chain ip6 test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule ip6 test prerouting exthdr type routing accept

chain prerouting {
    type nat hook prerouting priority 0;
}
sudo nft add rule ip test prerouting ip daddr 192.168.1.0/24 accept




sudo nft add table inet test
sudo nft add chain inet test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule inet test prerouting exthdr dst exists accept


sudo nft list ruleset

table inet test {
        chain prerouting {
                type filter hook prerouting priority filter; policy accept;
                exthdr dst exists accept
        }
}


sudo nft add rule ip6 test prerouting exthdr dst exists accept
sudo nft add rule ip6 test prerouting exthdr hopopts exists accept
sudo nft add rule ip6 test prerouting exthdr frag exists accept
sudo nft add rule ip6 test prerouting exthdr routing exists accept



git rebase -i db77c5b 
Первый из них оставляешь как pick, остальные 9 меняешь на squash (или s):

git rebase --abort


git push -u origin trace_core1 --force




_______________________________________________________________________________________________



package main

/*
   Полностью переписанный файл с буферизованным выводом.
   Логика IPv4/IPv6 сохранена без сокращений: обрабатываются
   все Sysexit-коды (1,2,3,6,11,12) и оба семейства адресов (2 и 10).
*/

import (
    "bytes"
    "context"
    "errors"
    "fmt"
    "log"
    "net"
    "os"
    "os/signal"
    "sync"
    "syscall"
    "time"
    "unsafe"

    "bpfgo/pkg"

    "github.com/cilium/ebpf/link"
    "github.com/cilium/ebpf/perf"
    "github.com/cilium/ebpf/rlimit"
)

// -----------------------------------------------------------------------------
//                         Буферизованный асинхронный логгер
// -----------------------------------------------------------------------------

const (
    logBufSize     = 8 << 10            // 8 KiB один флаш
    logChanSize    = 1 << 10            // 1024 сообщений в канале
    logFlushPeriod = 200 * time.Millisecond
)

var (
    logCh   = make(chan string, logChanSize)
    flushCh = make(chan struct{})
)

func startBufferedLogger(ctx context.Context) {
    go func() {
        var buf bytes.Buffer
        ticker := time.NewTicker(logFlushPeriod)
        defer ticker.Stop()

        flush := func() {
            if buf.Len() == 0 {
                return
            }
            _, _ = os.Stdout.Write(buf.Bytes())
            buf.Reset()
        }

        for {
            select {
            case msg, ok := <-logCh:
                if !ok {
                    flush()
                    return
                }
                buf.WriteString(msg)
                buf.WriteByte('\n')
                if buf.Len() >= logBufSize {
                    flush()
                }
            case <-ticker.C:
                flush()
            case <-flushCh:
                flush()
                return
            case <-ctx.Done():
                flush()
                return
            }
        }
    }()
}

func logf(format string, a ...any) {
    select {
    case logCh <- fmt.Sprintf(format, a...):
    default: // если канал переполнен — сбрасываем синхронно, чтобы не потерять данные
        fmt.Fprintf(os.Stdout, format+"\n", a...)
    }
}

// -----------------------------------------------------------------------------
//                           eBPF объекты и структуры
// -----------------------------------------------------------------------------

var objs bpfObjects

var (
    eventChanSport = make(chan int, 1)
    eventChanPID   = make(chan int, 1)
    mu             sync.Mutex
)

var (
    proto           string
    srcHost, dstHost string
)

// Event‑связывающие структуры

type Lookup struct {
    DstIP   net.IP
    DstPort int
    SrcIP   net.IP
    SrcPort int
    Proto   int
}

type Sendmsg struct {
    DstIP   net.IP
    DstPort int
    Pid     uint32
    Proto   int
    Comm    string
}

type Recvmsg struct {
    SrcIP   net.IP
    SrcPort int
    Pid     uint32
    Proto   int
    Comm    string
}

type EventData struct {
    Lookup  *Lookup
    Sendmsg *Sendmsg
    Recvmsg *Recvmsg
}

// -----------------------------------------------------------------------------
//                                init()
// -----------------------------------------------------------------------------

func init() {
    if err := rlimit.RemoveMemlock(); err != nil {
        log.Fatalf("remove memlock: %v", err)
    }
    if err := loadBpfObjects(&objs, nil); err != nil {
        log.Fatalf("load bpf objects: %v", err)
    }
}

// -----------------------------------------------------------------------------
//                             main()
// -----------------------------------------------------------------------------

func main() {
    ctx, cancel := context.WithCancel(context.Background())
    defer cancel()
    startBufferedLogger(ctx)

    // Graceful shutdown по Ctrl+C
    sigCh := make(chan os.Signal, 1)
    signal.Notify(sigCh, os.Interrupt, syscall.SIGTERM)
    go func() {
        <-sigCh
        close(flushCh) // попросим логер завершиться
        cancel()
    }()

    // Карты для корреляции событий
    eventMap := make(map[int]*EventData)
    eventMap1 := make(map[int]*EventData)

    // ---------------- eBPF Tracepoints & sk_lookup --------------------

    netns, err := os.Open("/proc/self/ns/net")
    if err != nil {
        log.Fatalf("open net namespace: %v", err)
    }
    defer netns.Close()

    attach := func(category, name string, prog *ebpfProgram) link.Link {
        l, err := link.Tracepoint(category, name, prog, nil)
        if err != nil {
            log.Fatalf("tracepoint %s:%s: %v", category, name, err)
        }
        return l
    }

    // Attach tracepoints — сокращаем boilerplate
    tp := []link.Link{
        attach("syscalls", "sys_enter_sendmsg", objs.TraceSendmsgEnter),
        attach("syscalls", "sys_exit_sendmsg", objs.TraceSendmsgExit),
        attach("syscalls", "sys_enter_sendto", objs.TraceSendtoEnter),
        attach("syscalls", "sys_exit_sendto", objs.TraceSendtoExit),
        attach("syscalls", "sys_enter_recvmsg", objs.TraceRecvmsgEnter),
        attach("syscalls", "sys_exit_recvmsg", objs.TraceRecvmsgExit),
        attach("syscalls", "sys_enter_recvfrom", objs.TraceRecvfromEnter),
        attach("syscalls", "sys_exit_recvfrom", objs.TraceRecvfromExit),
        attach("sock", "inet_sock_set_state", objs.TraceTcpEst),
    }
    defer func() { for _, l := range tp { l.Close() } }()

    skLookupLink, err := link.AttachNetNs(int(netns.Fd()), objs.LookUp)
    if err != nil {
        log.Fatalf("attach sk_lookup: %v", err)
    }
    defer skLookupLink.Close()

    // ---------------- perf.Reader goroutine --------------------------

    go func() {
        rd, err := perf.NewReader(objs.TraceEvents, 8192)
        if err != nil {
            log.Fatalf("perf reader: %v", err)
        }
        defer rd.Close()

        exe := os.Args[0]
        if len(exe) > 2 {
            exe = exe[2:]
        }

        for {
            select {
            case <-ctx.Done():
                return
            default:
            }

            record, err := rd.Read()
            if err != nil {
                if errors.Is(err, os.ErrDeadlineExceeded) {
                    continue
                }
                logf("perf read error: %v", err)
                return
            }

            if len(record.RawSample) < int(unsafe.Sizeof(bpfTraceInfo{})) {
                logf("invalid event size: %d", len(record.RawSample))
                continue
            }

            event := *(*bpfTraceInfo)(unsafe.Pointer(&record.RawSample[0]))

            // IPv4 адреса
            srcIP := net.IPv4(
                byte(event.SrcIP.S_addr),
                byte(event.SrcIP.S_addr>>8),
                byte(event.SrcIP.S_addr>>16),
                byte(event.SrcIP.S_addr>>24),
            )
            dstIP := net.IPv4(
                byte(event.DstIP.S_addr),
                byte(event.DstIP.S_addr>>8),
                byte(event.DstIP.S_addr>>16),
                byte(event.DstIP.S_addr>>24),
            )

            if pkg.Int8ToString(event.Comm) == exe {
                continue // пропускаем собственные события
            }

            // ---------------- Switch по Sysexit -----------------------

            switch event.Sysexit {
            case 1: // SENDTO exit
                handleSendtoExit(event, srcIP, dstIP, eventMap1, eventMap)
            case 11: // SENDMSG exit
                handleSendmsgExit(event, srcIP, dstIP, eventMap)
            case 2: // RECVFROM exit
                handleRecvfromExit(event, srcIP, dstIP, eventMap)
            case 12: // RECVMSG exit
                handleRecvmsgExit(event, srcIP, dstIP, eventMap)
            case 3: // sk_lookup result
                handleLookup(event, srcIP, dstIP, eventMap, eventMap1)
            case 6: // TCP state change
                handleTCPState(event, srcIP, dstIP)
            }
        }
    }()

    logf("Press Ctrl+C to exit")
    <-ctx.Done()
    logf("Exiting…")
}

// -----------------------------------------------------------------------------
//  Хендлеры для разных Sysexit-кодов (IPv4 и IPv6 без сокращений)
// -----------------------------------------------------------------------------

func handleSendtoExit(event bpfTraceInfo, srcIP, dstIP net.IP, eventMap1, eventMap map[int]*EventData) {
    if event.Family == 2 { // IPv4
        port := int(event.Dport)
        data := getOrCreate(eventMap1, port)
        data.Sendmsg = &Sendmsg{
            DstIP:   dstIP,
            DstPort: port,
            Pid:     event.Pid,
            Comm:    pkg.Int8ToString(event.Comm),
        }
        if data.Lookup != nil && data.Recvmsg != nil {
            proto := protoStr(data.Lookup.Proto)
            resolveHosts(data.Lookup)
            logf("SENDTO PID=%d NAME=%s %s/%s[%s]:%d -> %s[%s]:%d",
                data.Sendmsg.Pid, data.Sendmsg.Comm,
                proto, dstHost, data.Lookup.DstIP, data.Lookup.DstPort,
                srcHost, data.Lookup.SrcIP, data.Lookup.SrcPort,
            )
            logf("SENDTO PID=%d NAME=%s %s/%s[%s]:%d <- %s[%s]:%d",
                data.Recvmsg.Pid, data.Recvmsg.Comm,
                proto, dstHost, data.Lookup.DstIP, data.Lookup.DstPort,
                srcHost, data.Lookup.SrcIP, data.Lookup.SrcPort,
            )
            delete(eventMap, port)
        }
    } else if event.Family == 10 { // IPv6
        port := event.Dport
        pid := event.Pid
        ip6 := pkg.IPv6FromLEWords(IPv6BytesToWords(event.DstIP6.In6U.U6Addr8))
        logf("SENDTO IPv6 PID=%d IPv6=%s:%d NAME=%s", pid, ip6.String(), port, pkg.Int8ToString(event.Comm))
    }
}

func handleSendmsgExit(event bpfTraceInfo, srcIP, dstIP net.IP, eventMap map[int]*EventData) {
    if event.Family == 2 {
        port := int(event.Dport)
        data := getOrCreate(eventMap, port)
        data.Sendmsg = &Sendmsg{
            DstIP:   dstIP,
            DstPort: port,
            Pid:     event.Pid,
            Comm:    pkg.Int8ToString(event.Comm),
        }
        if data.Lookup != nil && data.Recvmsg != nil {
            proto := protoStr(data.Lookup.Proto)
            logf("")
            logf("SENDMSG PID=%d NAME=%s %s/%s:%d->%s:%d",
                data.Sendmsg.Pid, data.Sendmsg.Comm, proto,
                data.Lookup.DstIP, data.Lookup.DstPort,
                data.Lookup.SrcIP, data.Lookup.SrcPort)
            logf("SENDMSG PID=%d NAME=%s %s/%s:%d<-%s:%d",
                data.Recvmsg.Pid, data.Recvmsg.Comm, proto,
                data.Lookup.DstIP, data.Lookup.DstPort,
                data.Lookup.SrcIP, data.Lookup.SrcPort)
            logf("")
            delete(eventMap, port)
        }
    } else if event.Family == 10 {
        port := event.Dport
        pid := event.Pid
        ip6 := pkg.IPv6FromLEWords(IPv6BytesToWords(event.DstIP6.In6U.U6Addr8))
        logf("SENDMSG IPv6 PID=%d IPv6=%s:%d NAME=%s", pid, ip6.String(), port, pkg.Int8ToString(event.Comm))
    }
}

func handleRecvfromExit(event bpfTraceInfo, srcIP, dstIP net.IP, eventMap map[int]*EventData) {
    if event.Family == 2 {
        port := int(event.Sport)
        data := getOrCreate(eventMap, port)
        data.Recvmsg = &Recvmsg{
            SrcIP:   srcIP,
            SrcPort: port,
            Pid:     event.Pid,
            Comm:    pkg.Int8ToString(event.Comm),
        }
        if data.Lookup != nil && data.Sendmsg != nil {
            proto := protoStr(data.Lookup.Proto)
            resolveHosts(data.Lookup)
            logf("RECVFROM PID=%d NAME=%s %s/%s[%s]:%d -> %s[%s]:%d",
                data.Sendmsg.Pid, data.Sendmsg.Comm,
                proto, dstHost, data.Lookup.DstIP, data.Lookup.DstPort,
                srcHost, data.Lookup.SrcIP, data.Lookup.SrcPort)
            logf("RECVFROM PID=%d NAME=%s %s/%s[%s]:%d <- %s[%s]:%d",
                data.Recvmsg.Pid, data.Recvmsg.Comm,
                proto, dstHost, data.Lookup.DstIP, data.Lookup.DstPort,
                srcHost, data.Lookup.SrcIP, data.Lookup.SrcPort)
            delete(eventMap, port)
        }
    } else if event.Family == 10 {
        port := event.Sport
        pid := event.Pid
        ip6 := pkg.IPv6FromLEWords(IPv6BytesToWords(event.SrcIP6.In6U.U6Addr8))
        logf("RECVFROM IPv6 PID=%d IPv6=%s:%d NAME=%s", pid, ip6.String(), port, pkg.Int8ToString(event.Comm))
    }
}

func handleRecvmsgExit(event bpfTraceInfo, srcIP, dstIP net.IP, eventMap map[int]*EventData) {
    if event.Family == 2 {
        port := int(event.Sport)
        data := getOrCreate(eventMap, port)
        data.Recvmsg = &Recvmsg{
            SrcIP:   srcIP,
            SrcPort: port,
            Pid:     event.Pid,
            Comm:    pkg.Int8ToString(event.Com









