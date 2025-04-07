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

SEC("tracepoint/syscalls/sys_exit_recvmsg")
int trace_recvmsg_exit(struct sys_exit_recvmsg_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    bpf_printk("sys_exit_recvmsg pid=%d ret=%ld", pid, ret);

    // Проверяем, есть ли данные для PID
    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) {
        bpf_printk("No conn_info for pid=%d", pid);
        return 0;
    }

    if (ret < 0) {
        bpf_printk("recvmsg failed for PID=%d", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    // Получаем указатель на msghdr
    struct msghdr **addr_ptr = bpf_map_lookup_elem(&addrRecv_map, &pid);
    if (!addr_ptr) {
        bpf_printk("No addr_ptr for pid=%d", pid);
        return 0;
    }

    struct msghdr *msg;
    bpf_probe_read_user(&msg, sizeof(msg), *addr_ptr);

    if (!msg) {
        bpf_printk("msg is NULL for pid=%d", pid);
        return 0;
    }

    // Чтение msg_name
    void *name_ptr = NULL;
    bpf_probe_read_user(&name_ptr, sizeof(name_ptr), &msg->msg_name);

    // Если msg_name не NULL, проверяем его содержимое
    if (name_ptr) {
        // Мы предполагаем, что это sockaddr_in (IPv4)
        struct sockaddr_in sa = {};

        // Чтение только sin_family из адреса
        bpf_probe_read_user(&sa, sizeof(sa.sin_family), name_ptr);

        // Проверяем семейство адресов
        if (sa.sin_family == AF_INET) {
            bpf_printk("sys_exit_recvmsg FAMILY=AF_INET");
        } else {
            bpf_printk("Unknown family: %d", sa.sin_family);
        }
    }

    return 0;
}


msg_control
msg_control_is_user
msg_control_user
msg_controllen
msg_flags
msg_gen_inq
msg_inq
msg_iocb
msg_iter
msg_name
msg_namelen
msg_ubuf

STATE=12 IP4 PID=6603 dstIP=//[127.0.0.1]:60142 NAME=systemd-resolve
STATE=12 IP4 PID=6603 dstIP=//[192.168.1.1]:53 NAME=systemd-resolve
STATE=11 IP4 PID=6603  dstIP=//[127.0.0.1]:60142 FAMILY=2 NAME=systemd-resolve 
STATE=12 IP4 PID=6603 dstIP=//[127.0.0.1]:53347 NAME=systemd-resolve
STATE=12 IP4 PID=6603 dstIP=//[192.168.1.1]:53 NAME=systemd-resolve
STATE=11 IP4 PID=6603  dstIP=//[127.0.0.1]:53347 FAMILY=2 NAME=systemd-resolve 
STATE=12 IP4 PID=6603 dstIP=//[127.0.0.1]:36366 NAME=systemd-resolve
STATE=12 IP4 PID=6603 dstIP=//[192.168.1.1]:53 NAME=systemd-resolve
STATE=11 IP4 PID=6603  dstIP=//[127.0.0.1]:36366 FAMILY=2 NAME=systemd-resolve 
STATE=12 IP4 PID=6603 dstIP=//[127.0.0.1]:36003 NAME=systemd-resolve
STATE=11 IP4 PID=6603  dstIP=//[127.0.0.1]:36003 FAMILY=2 NAME=systemd-resolve 









