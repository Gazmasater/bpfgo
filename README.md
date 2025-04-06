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

sys_enter_recvmsg_args {
        unsigned short common_type;      
        unsigned char common_flags;      
        unsigned char common_preempt_count;     
        int common_pid;   

        int __syscall_nr; 
        int fd;
        int  pad;   
        struct user_msghdr * msg; 
        unsigned int flags;      

};


struct sys_exit_recvmsg_args{

        unsigned short common_type;       
        unsigned char common_flags;    
        unsigned char common_preempt_count;    
        int common_pid;   

        int __syscall_nr; 
        int pad;
        long ret; 

};

#include <linux/bpf.h>
#include <linux/ptrace.h>
#include <linux/in.h>
#include <linux/in6.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/socket.h>
#include <linux/uio.h>
#include <linux/version.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>

char LICENSE[] SEC("license") = "GPL";

struct conn_info_t {
    u32 pid;
    char comm[16];
};

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct conn_info_t);
} conn_info_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct msghdr *);
} addrRecv_map SEC(".maps");

SEC("tracepoint/syscalls/sys_enter_recvmsg")
int trace_recvmsg_enter(struct sys_enter_recvmsg_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    struct msghdr *addr = (struct msghdr *)ctx->msg;
    bpf_map_update_elem(&addrRecv_map, &pid, &addr, BPF_ANY);

    bpf_printk("sys_enter_recvmsg pid=%d addr=%p", pid, addr);
    return 0;
}

SEC("tracepoint/syscalls/sys_exit_recvmsg")
int trace_recvmsg_exit(struct sys_exit_recvmsg_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) return 0;

    if (ret < 0) {
        bpf_printk("recvmsg failed: pid=%d", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        bpf_map_delete_elem(&addrRecv_map, &pid);
        return 0;
    }

    struct msghdr **addr_ptr = bpf_map_lookup_elem(&addrRecv_map, &pid);
    if (!addr_ptr) {
        bpf_printk("addrRecv_map miss for pid=%d", pid);
        return 0;
    }

    struct msghdr *msg = *addr_ptr;
    if (!msg) {
        bpf_printk("null msg for pid=%d", pid);
        goto cleanup;
    }

    struct sockaddr_in sa = {};
    bpf_probe_read_user(&sa, sizeof(sa), msg->msg_name);

    if (sa.sin_family == AF_INET) {
        u32 ip = sa.sin_addr.s_addr;
        u16 port = sa.sin_port;
        bpf_printk("recvmsg from %d.%d.%d.%d:%d (pid=%d)",
            ((u8 *)&ip)[0], ((u8 *)&ip)[1],
            ((u8 *)&ip)[2], ((u8 *)&ip)[3],
            __bpf_ntohs(port), pid);
    } else {
        bpf_printk("recvmsg from unsupported family=%d (pid=%d)", sa.sin_family, pid);
    }

cleanup:
    bpf_map_delete_elem(&conn_info_map, &pid);
    bpf_map_delete_elem(&addrRecv_map, &pid);
    return 0;
}






