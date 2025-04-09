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



✅ Установка ядра 5.15.0-111-generic через jammy-proposed
Добавь jammy-proposed в sources.list:


echo "deb http://archive.ubuntu.com/ubuntu/ jammy-proposed main" | sudo tee -a /etc/apt/sources.list
Это откроет доступ к тестовым (предварительным) пакетам для Ubuntu 22.04 (Jammy).

Обнови индекс пакетов:


sudo apt-get update
Установи модули ядра:


sudo apt-get install linux-modules-5.15.0-111-generic
Также при необходимости можно установить и само ядро:


sudo apt-get install linux-image-5.15.0-111-generic
Перезагрузка (если устанавливал ядро):


sudo reboot
⚠️ Совет:
После установки можно удалить jammy-proposed, чтобы случайно не обновить другие пакеты из нестабильной ветки:

sudo sed -i '/jammy-proposed/d' /etc/apt/sources.list
sudo apt-get update



 sudo dpkg --configure -a
sudo apt-get install linux-modules-5.15.0-111-generic





gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$  sudo dpkg --configure -a
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo apt-get install linux-modules-5.15.0-111-generic
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
The following packages were automatically installed and are no longer required:
  amd64-microcode bpfcc-tools bpftrace ieee-data intel-microcode iucode-tool libbpfcc
  libllvm17t64 linux-headers-6.11.0-21-generic linux-headers-6.8.0-57
  linux-headers-generic-hwe-24.04 linux-hwe-6.11-headers-6.11.0-21 python3-bpfcc
  python3-netaddr thermald ubuntu-kernel-accessories
Use 'sudo apt autoremove' to remove them.
The following additional packages will be installed:
  linux-image-5.15.0-111-generic
Suggested packages:
  fdutils linux-doc | linux-source-5.15.0 linux-tools linux-headers-5.15.0-111-generic
  linux-modules-extra-5.15.0-111-generic
The following NEW packages will be installed:
  linux-image-5.15.0-111-generic linux-modules-5.15.0-111-generic
0 upgraded, 2 newly installed, 0 to remove and 235 not upgraded.
1 not fully installed or removed.
Need to get 34.2 MB of archives.
After this operation, 127 MB of additional disk space will be used.
Do you want to continue? [Y/n] Y
Get:1 http://archive.ubuntu.com/ubuntu jammy-proposed/main amd64 linux-modules-5.15.0-111-generic amd64 5.15.0-111.121 [22.6 MB]
Get:2 http://archive.ubuntu.com/ubuntu jammy-proposed/main amd64 linux-image-5.15.0-111-generic amd64 5.15.0-111.121 [11.5 MB]
Fetched 34.2 MB in 4s (8,516 kB/s)                         
debconf: unable to initialize frontend: Dialog
debconf: (Dialog frontend requires a screen at least 13 lines tall and 31 columns wide.)
debconf: falling back to frontend: Readline
Selecting previously unselected package linux-modules-5.15.0-111-generic.
(Reading database ... 212582 files and directories currently installed.)
Preparing to unpack .../linux-modules-5.15.0-111-generic_5.15.0-111.121_amd64.deb ...
Unpacking linux-modules-5.15.0-111-generic (5.15.0-111.121) ...
Selecting previously unselected package linux-image-5.15.0-111-generic.
Preparing to unpack .../linux-image-5.15.0-111-generic_5.15.0-111.121_amd64.deb ...
Unpacking linux-image-5.15.0-111-generic (5.15.0-111.121) ...
Setting up linux-image-6.8.0-47-generic (6.8.0-47.47) ...
I: /boot/vmlinuz.old is now a symlink to vmlinuz-5.15.0-111-generic
Setting up linux-image-5.15.0-111-generic (5.15.0-111.121) ...
I: /boot/vmlinuz.old is now a symlink to vmlinuz-6.8.0-47-generic
I: /boot/initrd.img.old is now a symlink to initrd.img-6.8.0-47-generic
I: /boot/vmlinuz is now a symlink to vmlinuz-5.15.0-111-generic
I: /boot/initrd.img is now a symlink to initrd.img-5.15.0-111-generic
Setting up linux-modules-5.15.0-111-generic (5.15.0-111.121) ...
Processing triggers for linux-image-6.8.0-47-generic (6.8.0-47.47) ...
/etc/kernel/postinst.d/initramfs-tools:
update-initramfs: Generating /boot/initrd.img-6.8.0-47-generic
/etc/kernel/postinst.d/zz-update-grub:
Sourcing file `/etc/default/grub'
Generating grub configuration file ...
Found linux image: /boot/vmlinuz-6.8.0-47-generic
Found initrd image: /boot/initrd.img-6.8.0-47-generic
Found linux image: /boot/vmlinuz-5.15.0-111-generic
Found memtest86+ 64bit EFI image: /boot/memtest86+x64.efi
Warning: os-prober will be executed to detect other bootable partitions.
Its output will be used to detect bootable binaries on them and create new boot entries.
Found Windows Boot Manager on /dev/nvme0n1p1@/EFI/Microsoft/Boot/bootmgfw.efi
Found Ubuntu 24.04.1 LTS (24.04) on /dev/nvme0n1p8
Found Ubuntu 24.04.1 LTS (24.04) on /dev/nvme0n1p9
Adding boot menu entry for UEFI Firmware Settings ...
done
Processing triggers for linux-image-5.15.0-111-generic (5.15.0-111.121) ...
/etc/kernel/postinst.d/initramfs-tools:
update-initramfs: Generating /boot/initrd.img-5.15.0-111-generic
/etc/kernel/postinst.d/zz-update-grub:
Sourcing file `/etc/default/grub'
Generating grub configuration file ...
Found linux image: /boot/vmlinuz-6.8.0-47-generic
Found initrd image: /boot/initrd.img-6.8.0-47-generic
Found linux image: /boot/vmlinuz-5.15.0-111-generic
Found initrd image: /boot/initrd.img-5.15.0-111-generic
Found memtest86+ 64bit EFI image: /boot/memtest86+x64.efi
Warning: os-prober will be executed to detect other bootable partitions.
Its output will be used to detect bootable binaries on them and create new boot entries.
Found Windows Boot Manager on /dev/nvme0n1p1@/EFI/Microsoft/Boot/bootmgfw.efi
Found Ubuntu 24.04.1 LTS (24.04) on /dev/nvme0n1p8
Found Ubuntu 24.04.1 LTS (24.04) on /dev/nvme0n1p9
Adding boot menu entry for UEFI Firmware Settings ...
done
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo sed -i '/jammy-proposed/d' /etc/apt/sources.list
^C
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ ^C
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ ^C
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ uname -r
6.8.0-47-generic
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo apt-get install linux-image-5.15.0-111-generic
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
linux-image-5.15.0-111-generic is already the newest version (5.15.0-111.121).
linux-image-5.15.0-111-generic set to manually installed.
The following packages were automatically installed and are no longer required:
  amd64-microcode bpfcc-tools bpftrace ieee-data intel-microcode iucode-tool libbpfcc
  libllvm17t64 linux-headers-6.11.0-21-generic linux-headers-6.8.0-57
  linux-headers-generic-hwe-24.04 linux-hwe-6.11-headers-6.11.0-21 python3-bpfcc
  python3-netaddr thermald ubuntu-kernel-accessories
Use 'sudo apt autoremove' to remove them.
0 upgraded, 0 newly installed, 0 to remove and 235 not upgraded.
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ uname -r
6.8.0-47-generic
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo sed -i '/jammy-proposed/d' /etc/apt/sources.list
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo apt-get update
Hit:1 http://ru.archive.ubuntu.com/ubuntu noble InRelease
Hit:2 http://ru.archive.ubuntu.com/ubuntu noble-updates InRelease
Hit:3 http://ru.archive.ubuntu.com/ubuntu noble-backports InRelease
Hit:4 http://security.ubuntu.com/ubuntu noble-security InRelease
Reading package lists... Done
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ uname -r
6.8.0-47-generic
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 





