lsns -t net


sudo ip netns list
ls /sys/kernel/tracing/events
ls /sys/kernel/tracing/events/net/




bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

bpftrace -l

sudo ss -tunp | grep 192.0.73.2:443


sudo apt update && sudo apt install -y tcpdump
sudo tcpdump -i wlp0s20f3 udp -nn

sudo tcpdump -i any -nn 'tcp[tcpflags] & (tcp-syn) != 0'

ls /sys/kernel/debug/tracing/events/sock/udp_sendmsg


srcAddr := fmt.Sprintf("%s:%d (%s)", srcIP.String(), event.Sport, ResolveIP(srcIP))
dstAddr := fmt.Sprintf("%s:%d (%s)", dstIP.String(), event.Dport, ResolveIP(dstIP))


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo ls /sys/kernel/tracing/events
alarmtimer        fs_dax          iwlwifi_msg    nvme            sof
amd_cpu           ftrace          iwlwifi_ucode  oom             sof_intel
asoc              gpio            jbd2           osnoise         spi
avc               gpu_scheduler   kmem           page_isolation  swiotlb
block             handshake       ksm            pagemap         sync_trace
bpf_test_run      hda             kvm            page_pool       syscalls
bpf_trace         hda_controller  kvmmmu         percpu          task
bridge            hda_intel       libata         power           tcp
cfg80211          header_event    lock           printk          thermal
cgroup            header_page     mac80211       pwm             thermal_power_allocator
clk               huge_memory     mac80211_msg   qdisc           thp
compaction        hwmon           maple_tree     qrtr            timer
context_tracking  hyperv          mce            ras             tlb
cpuhp             i2c             mctp           raw_syscalls    udp
cros_ec           i915            mdio           rcu             v4l2
csd               initcall        mei            regmap          vb2
dev               intel_iommu     migrate        regulator       vmalloc
devfreq           interconnect    mmap           resctrl         vmscan
devlink           iocost          mmap_lock      rpm             vsyscall
dma_fence         iomap           mmc            rseq            watchdog
drm               iommu           module         rtc             wbt
enable            io_uring        mptcp          rv              workqueue
error_report      ipi             msr            sched           writeback
exceptions        irq             napi           scsi            x86_fpu
ext4              irq_matrix      neigh          sd              xdp
fib               irq_vectors     net            signal          xe
fib6              iwlwifi         netlink        skb             xen
filelock          iwlwifi_data    nmi            smbus           xhci-hcd
filemap           iwlwifi_io      notifier       sock
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 


systemctl is-active systemd-resolved

sudo systemctl stop systemd-resolved
sudo systemctl start systemd-resolved

sudo systemctl disable systemd-resolved

ip route

Установка wireshark

sudo apt update
sudo apt install wireshark
sudo dpkg-reconfigure wireshark-common //без sudo
sudo usermod -aG wireshark $USER //добавить пользователя
wireshark //запуск

echo "Hello, UDP!" | nc -u -w1 34.117.188.166 443
echo "Hello, UDP!" | socat - UDP:34.117.188.166:443

Frame 108: 149 bytes on wire (1192 bits), 149 bytes captured (1192 bits) on interface wlp0s20f3, id 0
Ethernet II, Src: Intel_a8:ec:0b (e4:fd:45:a8:ec:0b), Dst: IPv6mcast_01:00:02 (33:33:00:01:00:02)
Internet Protocol Version 6, Src: fe80::d6b2:9200:15bb:a0e8, Dst: ff02::1:2
User Datagram Protocol, Src Port: 546, Dst Port: 547
DHCPv6

Frame 1024: 121 bytes on wire (968 bits), 121 bytes captured (968 bits) on interface wlp0s20f3, id 0
Ethernet II, Src: Sercomm_b7:d6:e8 (0c:73:29:b7:d6:e8), Dst: Intel_a8:ec:0b (e4:fd:45:a8:ec:0b)
Internet Protocol Version 6, Src: fe80::e73:29ff:feb7:d6e8, Dst: fe80::d6b2:9200:15bb:a0e8
User Datagram Protocol, Src Port: 49832, Dst Port: 546
DHCPv6

STATE1 IPv6 PID=982 IPv6=ff020000:0:0:10002:547
STATE=3 DST IPv6=fe800000:0:e7329ff:feb7d6e8
STATE=3 SRC IPv6=fe800000:0:d6b29200:15bba0e8
STATE=3 SPORT=546  DPORT=49832 PROTO=17


struct sys_enter_recvmsg_args {
        unsigned short common_type;       offset:0;       size:2; 
        unsigned char common_flags;       offset:2;       size:1; 
        unsigned char common_preempt_count;       offset:3;       size:1; 
        int common_pid;   offset:4;       size:4; 

        int __syscall_nr; offset:8;       size:4; 
        int fd;   offset:16;      size:8; 
        struct user_msghdr * msg; offset:24;      size:8; 
        unsigned int flags;       offset:32;      size:8; 

}


struct sys_exit_recvmsg_args{

        unsigned short common_type;       offset:0;       size:2; 
        unsigned char common_flags;       offset:2;       size:1; 
        unsigned char common_preempt_count;       offset:3;       size:1; 
        int common_pid;   offset:4;       size:4; signed:1;

        int __syscall_nr; offset:8;       size:4; 
        long ret; offset:16;      size:8;

}

SEC("tracepoint/syscalls/sys_enter_recvmsg")
int trace_recvmsg_enter(struct sys_enter_recvmsg_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};

    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    struct user_msghdr *msg = ctx->msg;

    if (!msg)
        return 0;

    struct sockaddr *addr = NULL;
    bpf_probe_read_user(&addr, sizeof(addr), &msg->msg_name);

    if (addr)
        bpf_map_update_elem(&addrRecv_map, &pid, &addr, BPF_ANY);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_recvmsg")
int trace_recvmsg_exit(struct sys_exit_recvmsg_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info)
        return 0;

    if (ret < 0) {
        bpf_printk("sys_exit_recvmsg failed for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct sockaddr **addr_ptr = bpf_map_lookup_elem(&addrRecv_map, &pid);
    if (!addr_ptr)
        return 0;

    struct sockaddr addr = {};
    bpf_probe_read_user(&addr, sizeof(addr), *addr_ptr);

    if (addr.sa_family == AF_INET) {
        struct sockaddr_in addr_in = {};
        bpf_probe_read_user(&addr_in, sizeof(addr_in), *addr_ptr);

        u32 ip = bpf_ntohl(addr_in.sin_addr.s_addr);
        u16 port = bpf_ntohs(addr_in.sin_port);

        struct trace_info info = {};
        info.pid = pid;
        __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));

        info.src_ip  = ip;
        info.sport   = port;
        info.family  = AF_INET;
        info.dst_ip  = 0;
        info.dport   = 0;
        info.sysexit = 2;

        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));
    } else if (addr.sa_family == AF_INET6) {
        struct sockaddr_in6 addr_in6 = {};
        bpf_probe_read_user(&addr_in6, sizeof(addr_in6), *addr_ptr);

        struct trace_info info = {};
        info.sysexit = 2;
        info.family  = AF_INET6;

        info.dstIP6[0] = bpf_ntohl(*(__u32 *)&addr_in6.sin6_addr.in6_u.u6_addr8[0]);
        info.dstIP6[1] = bpf_ntohl(*(__u32 *)&addr_in6.sin6_addr.in6_u.u6_addr8[4]);
        info.dstIP6[2] = bpf_ntohl(*(__u32 *)&addr_in6.sin6_addr.in6_u.u6_addr8[8]);
        info.dstIP6[3] = bpf_ntohl(*(__u32 *)&addr_in6.sin6_addr.in6_u.u6_addr8[12]);

        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));
    }

    bpf_map_delete_elem(&addrRecv_map, &pid);
    bpf_map_delete_elem(&conn_info_map, &pid);
    bpf_map_delete_elem(&addr_map, &pid); // если используется

    return 0;
}




