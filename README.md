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

\list

sudo -u gaz358 psql events_ebpf
\du
psql -U myuser -d syscalls

\dt
SELECT * FROM events;




import "strings"

// Функция для очистки строки от лишних пробелов и символов
func cleanString(input string) string {
    // Убираем лишние пробелы в начале и в конце строки
    cleaned := strings.TrimSpace(input)

    // Если нужно, можно добавить дополнительные очистки, например:
    // Убираем все лишние пробелы между словами
    cleaned = strings.Join(strings.Fields(cleaned), " ")

    return cleaned
}

func addStructure(eventID int, name string, format string, db *sql.DB) error {
    var id int

    // Очистка строк от лишних пробелов и символов
    name = cleanString(name)
    format = cleanString(format)

    // Логируем начало проверки структуры
    fmt.Printf("🔍 Проверка наличия структуры для события: %s (event_id = %d)\n", name, eventID)
    err := db.QueryRow(
        "SELECT id FROM structures WHERE event_id = $1 AND name = $2",
        eventID, name,
    ).Scan(&id)

    if err == sql.ErrNoRows {
        // Логируем, что структура не найдена
        fmt.Printf("❌ Структура не найдена, добавляем новую для события %s (event_id = %d)\n", name, eventID)

        // Логируем данные для вставки
        fmt.Printf("📋 Данные для вставки в структуру: event_id = %d, name = %s, format = %s\n", eventID, name, format)

        _, err := db.Exec(
            "INSERT INTO structures(event_id, name, format) VALUES($1, $2, $3)",
            eventID, name, format,
        )
        if err != nil {
            // Логируем ошибку, если вставка не удалась
            fmt.Printf("❌ Ошибка при добавлении структуры для события %s: %v\n", name, err)
            return fmt.Errorf("не удалось добавить структуру для события %s: %v", name, err)
        }

        // Логируем успешное добавление
        fmt.Printf("✅ Успешно добавлена структура для события %s (event_id = %d)\n", name, eventID)
    } else if err != nil {
        // Логируем ошибку, если при проверке структуры произошла ошибка
        fmt.Printf("❌ Ошибка при проверке структуры события %s: %v\n", name, err)
        return fmt.Errorf("не удалось проверить структуру события %s: %v", name, err)
    } else {
        // Логируем, что структура уже существует
        fmt.Printf("✔️ Структура для события %s уже существует\n", name)
    }

    return nil
}


