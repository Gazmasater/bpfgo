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



🔽 Установка старого ядра (например, 5.15.146-generic)
Перейди в каталог:


mkdir ~/kernel && cd ~/kernel
Скачай нужные .deb пакеты ядра (пример — 5.15.146, LTS и проверенное):


wget https://kernel.ubuntu.com/~kernel-ppa/mainline/v5.15.146/amd64/linux-headers-5.15.146-0515146_5.15.146-0515146.202403121036_all.deb
wget https://kernel.ubuntu.com/~kernel-ppa/mainline/v5.15.146/amd64/linux-headers-5.15.146-0515146-generic_5.15.146-0515146.202403121036_amd64.deb
wget https://kernel.ubuntu.com/~kernel-ppa/mainline/v5.15.146/amd64/linux-image-unsigned-5.15.146-0515146-generic_5.15.146-0515146.202403121036_amd64.deb
wget https://kernel.ubuntu.com/~kernel-ppa/mainline/v5.15.146/amd64/linux-modules-5.15.146-0515146-generic_5.15.146-0515146.202403121036_amd64.deb
Установи:


sudo dpkg -i *.deb
Обнови grub (если не обновится автоматически):


sudo update-grub
Перезагрузи:


reboot
На старте (если несколько ядер) — выбери нужное ядро через GRUB (обычно в меню «Advanced options for Ubuntu» → нужная версия ядра).

gaz358@gaz358-BOD-WXX9:~/kernel$ wget https://kernel.ubuntu.com/~kernel-ppa/mainline/v5.15.146/amd64/linux-modules-5.15.146-0515146-generic_5.15.146-0515146.202403121036_amd64.deb
--2025-04-09 15:56:25--  https://kernel.ubuntu.com/~kernel-ppa/mainline/v5.15.146/amd64/linux-modules-5.15.146-0515146-generic_5.15.146-0515146.202403121036_amd64.deb
Resolving kernel.ubuntu.com (kernel.ubuntu.com)... 185.125.189.75, 185.125.189.74, 185.125.189.76
Connecting to kernel.ubuntu.com (kernel.ubuntu.com)|185.125.189.75|:443... connected.
HTTP request sent, awaiting response... 301 Moved Permanently
Location: https://kernel.ubuntu.com/mainline/v5.15.146/amd64/linux-modules-5.15.146-0515146-generic_5.15.146-0515146.202403121036_amd64.deb [following]
--2025-04-09 15:56:26--  https://kernel.ubuntu.com/mainline/v5.15.146/amd64/linux-modules-5.15.146-0515146-generic_5.15.146-0515146.202403121036_amd64.deb
Reusing existing connection to kernel.ubuntu.com:443.
HTTP request sent, awaiting response... 404 Not Found
2025-04-09 15:56:26 ERROR 404: Not Found.



