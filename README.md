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





Если в GRUB меню не отображается ядро 5.15.0-111-generic, хотя ты его установил, это может быть связано с тем, что оно:

Установлено, но не попало в загрузочное меню;

Скрыто из-за особенностей grub.cfg;

Грузится через initrd с другим ядром;

Не подходит к текущей версии Ubuntu (у тебя — Ubuntu 24.04 noble, а ядро 5.15 — от Ubuntu 22.04 jammy).

🚀 Проверь, что ядро действительно установлено:

ls -l /boot/vmlinuz*
Если ты видишь:


/boot/vmlinuz-5.15.0-111-generic
значит, оно на месте.

🛠 Обновим GRUB вручную:

sudo update-grub
После этого перезагрузи систему:


sudo reboot
И снова в GRUB нажми Shift или Esc → Advanced options for Ubuntu → смотри, появилось ли ядро 5.15.0-111-generic.

❗️Если снова не появилось:
Можешь вручную прописать его в GRUB:

Открой файл:


sudo nano /etc/default/grub
Найди строку:


GRUB_DEFAULT=0
И замени на:

GRUB_DEFAULT="Advanced options for Ubuntu>Ubuntu, with Linux 5.15.0-111-generic"
Сохрани (Ctrl+O, Enter) и выйди (Ctrl+X), затем:


sudo update-grub
Перезагрузи:


sudo reboot
Если хочешь, я могу помочь точно указать, какой индекс использовать для GRUB_DEFAULT, чтобы не писать полное имя вручную — просто скинь сюда результат:


grep menuentry /boot/grub/grub.cfg
Так мы найдем нужную позицию в списке загрузки.

grep menuentry /boot/grub/grub.cfg | grep 5.15


✅ Шаг 1: Убедимся, как именно оно называется в GRUB
Покажи, пожалуйста, точный вывод команды:


grep menuentry /boot/grub/grub.cfg | grep 5.15
Например, ты можешь увидеть что-то вроде:


menuentry 'Ubuntu, with Linux 5.15.0-111-generic' --class ubuntu ...
Или:


menuentry 'Advanced options for Ubuntu>Ubuntu, with Linux 5.15.0-111-generic' ...
✅ Шаг 2: Укажи это в GRUB правильно
Если ты хочешь по умолчанию загружаться с ядром 5.15, тебе нужно сделать следующее:

Открой /etc/default/grub:


sudo nano /etc/default/grub
Найди строку GRUB_DEFAULT=... и замени её на:


GRUB_DEFAULT="Advanced options for Ubuntu>Ubuntu, with Linux 5.15.0-111-generic"
(Подставь точное имя из вывода команды grep)

Сохрани файл и выйди из nano (Ctrl + O → Enter, затем Ctrl + X).

✅ Шаг 3: Обнови конфигурацию GRUB

sudo update-grub
✅ Шаг 4: Перезагрузи

sudo reboot
После перезагрузки проверь:


uname -r




grep menuentry /boot/grub/grub.cfg

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo grep menuentry /boot/grub/grub.cfg
if [ x"${feature_menuentry_id}" = xy ]; then
  menuentry_id_option="--id"
  menuentry_id_option=""
export menuentry_id_option
menuentry 'Ubuntu' --class ubuntu --class gnu-linux --class gnu --class os $menuentry_id_option 'gnulinux-simple-203efa69-34e3-4d47-9f4c-bd68a0cbb9b6' {
submenu 'Advanced options for Ubuntu' $menuentry_id_option 'gnulinux-advanced-203efa69-34e3-4d47-9f4c-bd68a0cbb9b6' {
        menuentry 'Ubuntu, with Linux 6.8.0-47-generic' --class ubuntu --class gnu-linux --class gnu --class os $menuentry_id_option 'gnulinux-6.8.0-47-generic-advanced-203efa69-34e3-4d47-9f4c-bd68a0cbb9b6' {
        menuentry 'Ubuntu, with Linux 6.8.0-47-generic (recovery mode)' --class ubuntu --class gnu-linux --class gnu --class os $menuentry_id_option 'gnulinux-6.8.0-47-generic-recovery-203efa69-34e3-4d47-9f4c-bd68a0cbb9b6' {
        menuentry 'Ubuntu, with Linux 5.15.0-111-generic' --class ubuntu --class gnu-linux --class gnu --class os $menuentry_id_option 'gnulinux-5.15.0-111-generic-advanced-203efa69-34e3-4d47-9f4c-bd68a0cbb9b6' {
        menuentry 'Ubuntu, with Linux 5.15.0-111-generic (recovery mode)' --class ubuntu --class gnu-linux --class gnu --class os $menuentry_id_option 'gnulinux-5.15.0-111-generic-recovery-203efa69-34e3-4d47-9f4c-bd68a0cbb9b6' {
menuentry "Memory test (memtest86+x64.efi)" --class memtest $menuentry_id_option 'memtest86+' {
menuentry 'Memory test (memtest86+x64.efi, serial console)' --class memtest $menuentry_id_option 'memtest86+-serial' {
menuentry 'Windows Boot Manager (on /dev/nvme0n1p1)' --class windows --class os $menuentry_id_option 'osprober-efi-CE12-E24B' {
menuentry 'Ubuntu 24.04.1 LTS (24.04) (on /dev/nvme0n1p8)' --class ubuntu --class gnu-linux --class gnu --class os $menuentry_id_option 'osprober-gnulinux-simple-726c3c21-bed7-45dd-9ac0-784afade5b5d' {
submenu 'Advanced options for Ubuntu 24.04.1 LTS (24.04) (on /dev/nvme0n1p8)' $menuentry_id_option 'osprober-gnulinux-advanced-726c3c21-bed7-45dd-9ac0-784afade5b5d' {
        menuentry 'Ubuntu (on /dev/nvme0n1p8)' --class gnu-linux --class gnu --class os $menuentry_id_option 'osprober-gnulinux-/boot/vmlinuz-6.8.0-48-generic--726c3c21-bed7-45dd-9ac0-784afade5b5d' {
        menuentry 'Ubuntu, with Linux 6.8.0-48-generic (on /dev/nvme0n1p8)' --class gnu-linux --class gnu --class os $menuentry_id_option 'osprober-gnulinux-/boot/vmlinuz-6.8.0-48-generic--726c3c21-bed7-45dd-9ac0-784afade5b5d' {
        menuentry 'Ubuntu, with Linux 6.8.0-48-generic (recovery mode) (on /dev/nvme0n1p8)' --class gnu-linux --class gnu --class os $menuentry_id_option 'osprober-gnulinux-/boot/vmlinuz-6.8.0-48-generic-root=UUID=726c3c21-bed7-45dd-9ac0-784afade5b5d ro recovery nomodeset dis_ucode_ldr-726c3c21-bed7-45dd-9ac0-784afade5b5d' {
        menuentry 'Ubuntu, with Linux 6.8.0-47-generic (on /dev/nvme0n1p8)' --class gnu-linux --class gnu --class os $menuentry_id_option 'osprober-gnulinux-/boot/vmlinuz-6.8.0-47-generic--726c3c21-bed7-45dd-9ac0-784afade5b5d' {
        menuentry 'Ubuntu, with Linux 6.8.0-47-generic (recovery mode) (on /dev/nvme0n1p8)' --class gnu-linux --class gnu --class os $menuentry_id_option 'osprober-gnulinux-/boot/vmlinuz-6.8.0-47-generic-root=UUID=726c3c21-bed7-45dd-9ac0-784afade5b5d ro recovery nomodeset dis_ucode_ldr-726c3c21-bed7-45dd-9ac0-784afade5b5d' {
        menuentry 'Memory test (memtest86+x64.efi) (on /dev/nvme0n1p8)' --class gnu-linux --class gnu --class os $menuentry_id_option 'osprober-gnulinux-/boot/memtest86+x64.efi--726c3c21-bed7-45dd-9ac0-784afade5b5d' {
        menuentry 'Memory test (memtest86+x64.efi, serial console) (on /dev/nvme0n1p8)' --class gnu-linux --class gnu --class os $menuentry_id_option 'osprober-gnulinux-/boot/memtest86+x64.efi--726c3c21-bed7-45dd-9ac0-784afade5b5d' {
menuentry 'Ubuntu 24.04.1 LTS (24.04) (on /dev/nvme0n1p9)' --class ubuntu --class gnu-linux --class gnu --class os $menuentry_id_option 'osprober-gnulinux-simple-7ac4c3a2-cbf7-446f-a0a9-81c1d6f5d47a' {
submenu 'Advanced options for Ubuntu 24.04.1 LTS (24.04) (on /dev/nvme0n1p9)' $menuentry_id_option 'osprober-gnulinux-advanced-7ac4c3a2-cbf7-446f-a0a9-81c1d6f5d47a' {
        menuentry 'Ubuntu (on /dev/nvme0n1p9)' --class gnu-linux --class gnu --class os $menuentry_id_option 'osprober-gnulinux-/boot/vmlinuz-6.8.0-49-generic--7ac4c3a2-cbf7-446f-a0a9-81c1d6f5d47a' {
        menuentry 'Ubuntu, with Linux 6.8.0-49-generic (on /dev/nvme0n1p9)' --class gnu-linux --class gnu --class os $menuentry_id_option 'osprober-gnulinux-/boot/vmlinuz-6.8.0-49-generic--7ac4c3a2-cbf7-446f-a0a9-81c1d6f5d47a' {
        menuentry 'Ubuntu, with Linux 6.8.0-49-generic (recovery mode) (on /dev/nvme0n1p9)' --class gnu-linux --class gnu --class os $menuentry_id_option 'osprober-gnulinux-/boot/vmlinuz-6.8.0-49-generic-root=UUID=7ac4c3a2-cbf7-446f-a0a9-81c1d6f5d47a ro recovery nomodeset dis_ucode_ldr-7ac4c3a2-cbf7-446f-a0a9-81c1d6f5d47a' {
        menuentry 'Ubuntu, with Linux 6.8.0-48-generic (on /dev/nvme0n1p9)' --class gnu-linux --class gnu --class os $menuentry_id_option 'osprober-gnulinux-/boot/vmlinuz-6.8.0-48-generic--7ac4c3a2-cbf7-446f-a0a9-81c1d6f5d47a' {
        menuentry 'Ubuntu, with Linux 6.8.0-48-generic (recovery mode) (on /dev/nvme0n1p9)' --class gnu-linux --class gnu --class os $menuentry_id_option 'osprober-gnulinux-/boot/vmlinuz-6.8.0-48-generic-root=UUID=7ac4c3a2-cbf7-446f-a0a9-81c1d6f5d47a ro recovery nomodeset dis_ucode_ldr-7ac4c3a2-cbf7-446f-a0a9-81c1d6f5d47a' {
        menuentry 'Memory test (memtest86+x64.efi) (on /dev/nvme0n1p9)' --class gnu-linux --class gnu --class os $menuentry_id_option 'osprober-gnulinux-/boot/memtest86+x64.efi--7ac4c3a2-cbf7-446f-a0a9-81c1d6f5d47a' {
        menuentry 'Memory test (memtest86+x64.efi, serial console) (on /dev/nvme0n1p9)' --class gnu-linux --class gnu --class os $menuentry_id_option 'osprober-gnulinux-/boot/memtest86+x64.efi--7ac4c3a2-cbf7-446f-a0a9-81c1d6f5d47a' {
                menuentry 'UEFI Firmware Settings' $menuentry_id_option 'uefi-firmware' {
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 

















