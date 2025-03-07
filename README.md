gcc udp_server.c -o udp_server

Sockops, err := link.AttachCgroup(link.CgroupOptions{
    Path: "/sys/fs/cgroup",
})
if err != nil {
    log.Fatalf("opening tracepoint sockops: %s", err)
}
defer Sockops.Close()

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo ./bpfgo
[sudo] password for gaz358: 
panic: runtime error: invalid memory address or nil pointer dereference
[signal SIGSEGV: segmentation violation code=0x1 addr=0x10 pc=0x59136e]

goroutine 1 [running]:
github.com/cilium/ebpf.(*Program).FD(...)
        /home/gaz358/go/pkg/mod/github.com/cilium/ebpf@v0.17.3/prog.go:593
github.com/cilium/ebpf/link.AttachRawLink({0x28, 0x0, 0x0, 0x0, 0x0})
        /home/gaz358/go/pkg/mod/github.com/cilium/ebpf@v0.17.3/link/link.go:334 +0x6e
github.com/cilium/ebpf/link.newLinkCgroup(0x61a2de?, 0xe?, 0x0?)
        /home/gaz358/go/pkg/mod/github.com/cilium/ebpf@v0.17.3/link/cgroup.go:180 +0x88
github.com/cilium/ebpf/link.AttachCgroup({{0x61a2de?, 0x0?}, 0x61a2d0?, 0x0?})
        /home/gaz358/go/pkg/mod/github.com/cilium/ebpf@v0.17.3/link/cgroup.go:57 +0x119
main.main()
        /home/gaz358/myprog/bpfgo/main.go:98 +0x8a5
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 

