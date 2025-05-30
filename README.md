
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

import (
    "bpfgo/pkg"
    "errors"
    "fmt"
    "log"
    "net"
    "os"
    "os/signal"
    "sync"
    "syscall"
    "unsafe"

    "github.com/cilium/ebpf/link"
    "github.com/cilium/ebpf/perf"
    "github.com/cilium/ebpf/rlimit"
)

// Глобальные объекты BPF
var objs bpfObjects

var eventChan_sport = make(chan int, 1)
var eventChan_pid   = make(chan int, 1)

var mu sync.Mutex
var xxx, xxx_pid int
var proto, srchost, dsthost string

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

// ------------------  Новый, value‑ориентированный EventData  ------------------
type EventData struct {
    Lookup      Lookup
    Sendmsg     Sendmsg
    Recvmsg     Recvmsg
    haveLookup  bool
    haveSendmsg bool
    haveRecvmsg bool
}
// ----------------------------------------------------------------------------

func init() {
    // снимаем ограничение на память
    if err := rlimit.RemoveMemlock(); err != nil {
        log.Fatalf("failed to remove rlimit: %v", err)
    }
    // грузим eBPF
    if err := loadBpfObjects(&objs, nil); err != nil {
        log.Fatalf("failed to load bpf objects: %v", err)
    }
}

func main() {
    eventMap   := make(map[int]*EventData)
    eventMap_1 := make(map[int]*EventData)

    defer objs.Close()

    netns, err := os.Open("/proc/self/ns/net")
    if err != nil { log.Fatalf("open ns: %v", err) }
    defer netns.Close()

    // привязка tracepoints (опущена — без изменений)…
    // ---------------------------------------------------------------------

    skLookupLink, err := link.AttachNetNs(int(netns.Fd()), objs.LookUp)
    if err != nil { log.Fatalf("attach sk_lookup: %v", err) }
    defer skLookupLink.Close()

    stop := make(chan os.Signal, 1)
    signal.Notify(stop, os.Interrupt, syscall.SIGTERM)

    go func() {
        const buffLen = 8192
        rd, err := perf.NewReader(objs.TraceEvents, buffLen)
        if err != nil { log.Fatalf("perf reader: %v", err) }
        defer rd.Close()

        selfExe := os.Args[0]
        if len(selfExe) > 2 { selfExe = selfExe[2:] }

        for {
            rec, err := rd.Read()
            if err != nil {
                if errors.Is(err, os.ErrDeadlineExceeded) { continue }
                log.Printf("perf read: %v", err); return
            }
            if len(rec.RawSample) < int(unsafe.Sizeof(bpfTraceInfo{})) { continue }
            ev := *(*bpfTraceInfo)(unsafe.Pointer(&rec.RawSample[0]))

            if pkg.Int8ToString(ev.Comm) == selfExe { continue }

            srcIP := net.IPv4(
                byte(ev.SrcIP.S_addr),   byte(ev.SrcIP.S_addr>>8),
                byte(ev.SrcIP.S_addr>>16), byte(ev.SrcIP.S_addr>>24))
            dstIP := net.IPv4(
                byte(ev.DstIP.S_addr),   byte(ev.DstIP.S_addr>>8),
                byte(ev.DstIP.S_addr>>16), byte(ev.DstIP.S_addr>>24))

            switch ev.Sysexit {
            // ------------------- SENDTO exit -------------------
            case 1:
                if ev.Family != 2 { break }
                port := int(ev.Dport)
                data, ok := eventMap_1[port]
                if !ok { data = &EventData{}; eventMap_1[port] = data }

                data.Sendmsg = Sendmsg{ DstIP: dstIP, DstPort: port, Pid: ev.Pid, Comm: pkg.Int8ToString(ev.Comm) }
                data.haveSendmsg = true

                if data.haveLookup && data.haveRecvmsg {
                    outputSendRecv("SENDTO", data)
                    delete(eventMap_1, port)
                }

            // ------------------- SENDMSG exit ------------------
            case 11:
                if ev.Family != 2 { break }
                port := int(ev.Dport)
                data, ok := eventMap[port]
                if !ok { data = &EventData{}; eventMap[port] = data }

                data.Sendmsg = Sendmsg{ DstIP: dstIP, DstPort: port, Pid: ev.Pid, Comm: pkg.Int8ToString(ev.Comm) }
                data.haveSendmsg = true

                if data.haveLookup && data.haveRecvmsg {
                    fmt.Println("")
                    outputSendRecv("SENDMSG", data)
                    fmt.Println("")
                    delete(eventMap, port)
                }

            // ------------------- RECVFROM exit -----------------
            case 2:
                if ev.Family != 2 { break }
                port := int(ev.Sport)
                data, ok := eventMap[port]
                if !ok { data = &EventData{}; eventMap[port] = data }

                data.Recvmsg = Recvmsg{ SrcIP: srcIP, SrcPort: port, Pid: ev.Pid, Comm: pkg.Int8ToString(ev.Comm) }
                data.haveRecvmsg = true

                if data.haveLookup && data.haveSendmsg {
                    outputSendRecv("RECVFROM", data)
                    delete(eventMap, port)
                }

            // ------------------- RECVMSG exit ------------------
            case 12:
                if ev.Family != 2 { break }
                port := int(ev.Sport)
                data, ok := eventMap[port]
                if !ok { data = &EventData{}; eventMap[port] = data }

                data.Recvmsg = Recvmsg{ SrcIP: srcIP, SrcPort: port, Pid: ev.Pid, Comm: pkg.Int8ToString(ev.Comm) }
                data.haveRecvmsg = true

                if data.haveLookup && data.haveSendmsg {
                    fmt.Println("")
                    outputSendRecv("RECVMSG", data)
                    fmt.Println("")
                    delete(eventMap, port)
                }

            // ------------------- sk_lookup verdict ------------
            case 3:
                if ev.Family != 2 { break }
                port := int(ev.Dport)
                data, ok := eventMap[port]
                if !ok { data = &EventData{}; eventMap[port] = data }

                data.Lookup = Lookup{ SrcIP: srcIP, SrcPort: int(ev.Sport), DstIP: dstIP, DstPort: int(ev.Dport), Proto: int(ev.Proto) }
                data.haveLookup = true

                // зеркальная карта по sport → для обратного направления
                revPort := int(ev.Sport)
                d2, ok2 := eventMap_1[revPort]
                if !ok2 { d2 = &EventData{}; eventMap_1[revPort] = d2 }
                d2.Lookup = data.Lookup; d2.haveLookup = true

                if data.haveSendmsg && data.haveRecvmsg {
                    fmt.Println("")
                    outputSendRecv("LOOKUP", data)
                    fmt.Println("")
                    delete(eventMap, port)
                }
            }
        }
    }()

    fmt.Println("Press Ctrl+C to exit")
    <-stop
    fmt.Println("Exiting…")
}

func outputSendRecv(tag string, d *EventData) {
    if d.Lookup.Proto == 17 { proto = "UDP" } else { proto = "TCP" }

    if d.Lookup.DstIP.IsLoopback() { dsthost = "localhost" } else { dsthost = pkg.ResolveIP(d.Lookup.DstIP) }
    if d.Lookup.SrcIP.IsLoopback() { srchost = "localhost" } else { srchost = pkg.ResolveIP(d.Lookup.SrcIP) }

    fmt.Printf("%s PID=%d NAME=%s %s/%s[%s]:%d -> %s[%s]:%d\n",
        tag,
        d.Sendmsg.Pid,
        d.Sendmsg.Comm,
        proto,
        dsthost,
        d.Lookup.DstIP,
        d.Lookup.DstPort,
        srchost,
        d.Lookup.SrcIP,
        d.Lookup.SrcPort,
    )
    fmt.Printf("%s PID=%d NAME=%s %s/%s[%s]:%d <- %s[%s]:%d\n",
        tag,
        d.Recvmsg.Pid,
        d.Recvmsg.Comm,
        proto,
        dsthost,
        d.Lookup.DstIP,
        d.Lookup.DstPort,
        srchost,
        d.Lookup.SrcIP,
        d.Lookup.SrcPort,
    )
}

func IPv6BytesToWords(addr [16]uint8) [4]uint32 {
    var w [4]uint32
    for i := 0; i < 4; i++ {
        w[i] = uint32(addr[i*4]) |
            uint32(addr[i*4+1])<<8 |
            uint32(addr[i*4+2])<<16 |
            uint32(addr[i*4+3])<<24
    }
    return w
}













