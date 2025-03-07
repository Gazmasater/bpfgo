gcc udp_server.c -o udp_server

Sockops, err := link.AttachCgroup(link.CgroupOptions{
    Path: "/sys/fs/cgroup",
})
if err != nil {
    log.Fatalf("opening tracepoint sockops: %s", err)
}
defer Sockops.Close()

