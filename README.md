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


az358@gaz358-BOD-WXX9:~/myprog/bpfgo$ uname -r
6.8.0-47-generic


🔍 Посмотреть установленные ядра
Выполни:


dpkg --list | grep linux-image
Ты увидишь примерно такой список:


ii  linux-image-5.15.0-92-generic ...
ii  linux-image-6.8.0-47-generic ...
ii  linux-image-generic ...
Также полезно:

bash
Копировать
Редактировать
dpkg --list | grep linux-headers
📌 Узнать активное (текущее) ядро
bash
Копировать
Редактировать
uname -r
Пример вывода:


6.8.0-47-generic
Это ядро не стоит удалять, пока ты не перезагрузился в другое.

🧹 Удалить лишние ядра
1. Перезагрузись в нужное ядро (например, 5.15)
При старте системы удерживай Shift или Esc → выбери Advanced options → выбери ядро 5.15.0-...

Проверь:


uname -r
Должно быть:


5.15.0-92-generic
2. Удали старое ядро
Теперь можно удалить 6.8.0-47-generic:


sudo apt remove --purge linux-image-6.8.0-47-generic linux-headers-6.8.0-47-generic
Если установлены метапакеты, можешь также удалить:


sudo apt remove --purge linux-modules-6.8.0-47-generic
Или посмотреть полный список и удалить всё вручную:

sudo dpkg -P linux-image-6.8.0-47-generic linux-headers-6.8.0-47-generic linux-modules-6.8.0-47-generic
⚙️ После удаления
Обнови загрузчик:


sudo update-grub
И перезагрузись, чтобы убедиться, что всё в порядке.










