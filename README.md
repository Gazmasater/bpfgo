lsns -t net


sudo ip netns list
ls /sys/kernel/tracing/events
ls /sys/kernel/tracing/events/net/




bpf2go -output-dir . -tags linux -type trace_info -go-package=main -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

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


wget https://raw.githubusercontent.com/pimlie/ubuntu-mainline-kernel.sh/master/ubuntu-mainline-kernel.sh
chmod +x ubuntu-mainline-kernel.sh
sudo mv ubuntu-mainline-kernel.sh /usr/local/bin/
ubuntu-mainline-kernel.sh
ubuntu-mainline-kernel.sh -l
ubuntu-mainline-kernel.sh -i v5.15.132
ubuntu-mainline-kernel.sh -u





Для установки PostgreSQL на Linux, следуйте этим шагам:

1. Установка PostgreSQL на Ubuntu/Debian
Обновите список пакетов:


sudo apt update
Установите PostgreSQL:


sudo apt install postgresql postgresql-contrib
Проверьте статус службы PostgreSQL:

После установки служба PostgreSQL должна автоматически запуститься. Проверьте статус с помощью команды:


sudo systemctl status postgresql
Если PostgreSQL не запущен, вы можете запустить его вручную:


sudo systemctl start postgresql
Убедитесь, что PostgreSQL работает:

Для этого выполните команду:


psql --version
Вы должны увидеть версию PostgreSQL, например: psql (PostgreSQL) 13.3.


3. Основные команды для работы с PostgreSQL:
Подключение к базе данных PostgreSQL:

Чтобы подключиться к PostgreSQL, используйте команду psql:


sudo -u postgres psql
Создание базы данных:

Для создания базы данных выполните команду:


sudo -u postgres createdb mydatabase
Создание пользователя:

Для создания нового пользователя:


sudo -u postgres createuser myuser --pwprompt --interactive
Изменение пароля пользователя:

Чтобы изменить пароль пользователя myuser:


sudo -u postgres psql
ALTER USER myuser WITH PASSWORD 'newpassword';
Выйти из PostgreSQL:

Чтобы выйти из консоли PostgreSQL, выполните команду:


\q
4. Управление PostgreSQL с помощью systemctl
Запуск службы PostgreSQL:


sudo systemctl start postgresql
Остановка службы PostgreSQL:


sudo systemctl stop postgresql
Перезапуск службы PostgreSQL:


sudo systemctl restart postgresql

\list

sudo -u gaz358 psql events_ebpf
\du
psql -U myuser -d syscalls

\dt
DELETE FROM structures;

SELECT * FROM events;





SELECT id, name FROM structures
WHERE id IN (5101, 5102, 5103, 5876, 5877, 6622, 6623, 6624, 6625, 6626);


saddr
5101 | tls_alert_recv
 5102 | tls_alert_send
 5103 | tls_contenttype
 5876 | inet_sk_error_report
 5877 | inet_sock_set_state
 6622 | tcp_bad_csum
 6623 | tcp_cong_state_set
 6624 | tcp_destroy_sock
 6625 | tcp_probe
 6626 | tcp_rcv_space_adjust

sprort
 5033 | fib_table_lookup
 5034 | fib6_table_lookup
 5689 | net_dev_start_xmit
 5876 | inet_sk_error_report
 5877 | inet_sock_set_state
 6623 | tcp_cong_state_set
 6624 | tcp_destroy_sock
 6625 | tcp_probe
 6626 | tcp_rcv_space_adjust
 6627 | tcp_receive_reset

protocol
 5438 | kvm_vmgexit_msr_protocol_enter
 5439 | kvm_vmgexit_msr_protocol_exit
 5484 | ata_sff_hsm_command_complete
 5485 | ata_sff_hsm_state
 5487 | ata_sff_port_intr
 5684 | napi_gro_frags_entry
 5686 | napi_gro_receive_entry
 5689 | net_dev_start_xmit
 5693 | netif_receive_skb_entry
 5695 | netif_receive_skb_list_entry
 5698 | netif_rx_entry
 5870 | kfree_skb
 5872 | smbus_read
 5873 | smbus_reply
 5874 | smbus_result
 5875 | smbus_write



 gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo cat /sys/kernel/debug/tracing/events/net/netif_receive_skb_entry/format
name: netif_receive_skb_entry
ID: 1618
format:
        field:unsigned short common_type;       offset:0;       size:2; signed:0;
        field:unsigned char common_flags;       offset:2;       size:1; signed:0;
        field:unsigned char common_preempt_count;       offset:3;       size:1; signed:0;
        field:int common_pid;   offset:4;       size:4; signed:1;

        field:__data_loc char[] name;   offset:8;       size:4; signed:0;
        field:unsigned int napi_id;     offset:12;      size:4; signed:0;
        field:u16 queue_mapping;        offset:16;      size:2; signed:0;
        field:const void * skbaddr;     offset:24;      size:8; signed:0;
        field:bool vlan_tagged; offset:32;      size:1; signed:0;
        field:u16 vlan_proto;   offset:34;      size:2; signed:0;
        field:u16 vlan_tci;     offset:36;      size:2; signed:0;
        field:u16 protocol;     offset:38;      size:2; signed:0;
        field:u8 ip_summed;     offset:40;      size:1; signed:0;
        field:u32 hash; offset:44;      size:4; signed:0;
        field:bool l4_hash;     offset:48;      size:1; signed:0;
        field:unsigned int len; offset:52;      size:4; signed:0;
        field:unsigned int data_len;    offset:56;      size:4; signed:0;
        field:unsigned int truesize;    offset:60;      size:4; signed:0;
        field:bool mac_header_valid;    offset:64;      size:1; signed:0;
        field:int mac_header;   offset:68;      size:4; signed:1;
        field:unsigned char nr_frags;   offset:72;      size:1; signed:0;
        field:u16 gso_size;     offset:74;      size:2; signed:0;
        field:u16 gso_type;     offset:76;      size:2; signed:0;

print fmt: "dev=%s napi_id=%#x queue_mapping=%u skbaddr=%p vlan_tagged=%d vlan_proto=0x%04x vlan_tci=0x%04x protocol=0x%04x ip_summed=%d hash=0x%08x l4_hash=%d len=%u data_len=%u truesize=%u mac_header_valid=%d mac_header=%d nr_frags=%d gso_size=%d gso_type=%#x", __get_str(name), REC->napi_id, REC->queue_mapping, REC->skbaddr, REC->vlan_tagged, REC->vlan_proto, REC->vlan_tci, REC->protocol, REC->ip_summed, REC->hash, REC->l4_hash, REC->len, REC->data_len, REC->truesize, REC->mac_header_valid, REC->mac_header, REC->nr_frags, REC->gso_size, REC->gso_type
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo cat /sys/kernel/debug/tracing/events/net/netif_receive_skb_exit/format
name: netif_receive_skb_exit
ID: 1613
format:
        field:unsigned short common_type;       offset:0;       size:2; signed:0;
        field:unsigned char common_flags;       offset:2;       size:1; signed:0;
        field:unsigned char common_preempt_count;       offset:3;       size:1; signed:0;
        field:int common_pid;   offset:4;       size:4; signed:1;

        field:int ret;  offset:8;       size:4; signed:1;

print fmt: "ret=%d", REC->ret
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 


name: net_dev_start_xmit
ID: 1626
format:
        field:unsigned short common_type;       offset:0;       size:2; signed:0;
        field:unsigned char common_flags;       offset:2;       size:1; signed:0;
        field:unsigned char common_preempt_count;       offset:3;       size:1; signed:0;
        field:int common_pid;   offset:4;       size:4; signed:1;

        field:__data_loc char[] name;   offset:8;       size:4; signed:0;
        field:u16 queue_mapping;        offset:12;      size:2; signed:0;
        field:const void * skbaddr;     offset:16;      size:8; signed:0;
        field:bool vlan_tagged; offset:24;      size:1; signed:0;
        field:u16 vlan_proto;   offset:26;      size:2; signed:0;
        field:u16 vlan_tci;     offset:28;      size:2; signed:0;
        field:u16 protocol;     offset:30;      size:2; signed:0;
        field:u8 ip_summed;     offset:32;      size:1; signed:0;
        field:unsigned int len; offset:36;      size:4; signed:0;
        field:unsigned int data_len;    offset:40;      size:4; signed:0;
        field:int network_offset;       offset:44;      size:4; signed:1;
        field:bool transport_offset_valid;      offset:48;      size:1; signed:0;
        field:int transport_offset;     offset:52;      size:4; signed:1;
        field:u8 tx_flags;      offset:56;      size:1; signed:0;
        field:u16 gso_size;     offset:58;      size:2; signed:0;
        field:u16 gso_segs;     offset:60;      size:2; signed:0;
        field:u16 gso_type;     offset:62;      size:2; signed:0;


echo 1 | sudo tee /sys/kernel/debug/tracing/events/net/netif_receive_skb_entry/enable
sudo cat /sys/kernel/debug/tracing/trace_pipe

ping 8.8.8.8

sudo perf record -e net:netif_receive_skb_entry -a



ls /sys/kernel/debug/tracing/events/net/netif_receive_skb_entry/


nc -u -l 9999


echo "hello UDP" | nc -u 192.168.1.71 9999

ip a
echo "test" | nc -u 192.168.1.71 9999


SEC("tracepoint/net/netif_receive_skb_entry")
int trace_netif_receive_skb(struct netif_receive_skb_entry_args *ctx)
{
    bpf_printk("skb received:\n");
    struct trace_info info = {};

    info.sysexit=13;
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

    return 0;
}




1. Установи libtraceevent:
Если ты на Ubuntu / Debian:


sudo apt update
sudo apt install libtraceevent1 libtraceevent-dev
А чтобы perf пересобрался с поддержкой tracepoint, можно установить:


sudo apt install linux-tools-$(uname -r)
2. Проверь после установки:

perf list | grep netif_receive_skb_entry
Если всё ок, ты увидишь:


  net:netif_receive_skb_entry [Tracepoint]


bpftool gen trace > trace_helpers.h

sudo apt install -y clang llvm libelf-dev gcc make libbpf-dev libz-dev flex bison
git clone --depth=1 https://github.com/torvalds/linux.git
cd linux/tools/bpf/bpftool
make
sudo make install
bpftool gen trace > trace_helpers.h


ID=468 TCP://gaz358-BOD-WXX9[192.168.1.71]:43036 <- TCP://Unknown[94.100.180.211]:443 
PID=6947 TCP://gaz358-BOD-WXX9[192.168.1.71]:43036 -> TCP://limgsko.mail.ru.[94.100.180.211]:443 

func ResolveIP(ip net.IP) string {
	names, err := net.LookupAddr(ip.String())
	if err != nil || len(names) == 0 {
		return "Unknown"
	}
	return names[0]
}








