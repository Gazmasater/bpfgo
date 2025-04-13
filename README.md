lsns -t net


sudo ip netns list
ls /sys/kernel/tracing/events
ls /sys/kernel/tracing/events/net/




bpf2go -output-dir . -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

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


CREATE TABLE categories (
    id SERIAL PRIMARY KEY,
    name TEXT UNIQUE NOT NULL
);

CREATE TABLE events (
    id SERIAL PRIMARY KEY,
    name TEXT NOT NULL,
    category_id INT REFERENCES categories(id),
    UNIQUE(name, category_id)
);

