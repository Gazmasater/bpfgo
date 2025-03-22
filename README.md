lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


https://arthurchiao.art/blog/pidfd-and-socket-lookup-bpf-illustrated/


if event.Proto == 6 || event.Proto == 17 {
    fmt.Printf("PID=%d Comm=%s ,SrcIP: %s(%s), SrcPort: %d -> DstIP: %s(%s), DstPort: %d PROTO: %d\n",
        event.Pid,
        pkg.Int8ToString(event.Comm),
        srcIP.String(),
        pkg.ResolveIP(srcIP),
        event.Sport,
        dstIP.String(),
        pkg.ResolveIP(dstIP),
        event.Dport,
        event.Proto,
    )
}


