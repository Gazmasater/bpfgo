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


package main

import (
	"database/sql"
	"fmt"
	"io/ioutil"
	"log"
	"path/filepath"

	_ "github.com/lib/pq" // PostgreSQL driver
)

const (
	host     = "localhost"
	port     = 5432
	user     = "gaz358"
	password = "qwert"
	dbname   = "events_ebpf"
)

func main() {
	// Строка подключения к базе данных
	psqlInfo := fmt.Sprintf("host=%s port=%d user=%s password=%s dbname=%s sslmode=disable", host, port, user, password, dbname)

	// Открытие подключения
	db, err := sql.Open("postgres", psqlInfo)
	if err != nil {
		log.Fatal("Ошибка подключения к базе данных: ", err)
	}
	defer db.Close()

	// Проверка подключения
	err = db.Ping()
	if err != nil {
		log.Fatal("Не удалось подключиться к базе данных: ", err)
	}

	// Путь к папке с событиями
	eventsDir := "/sys/kernel/debug/tracing/events/"

	// Обработка событий
	err = processEvents(eventsDir, db)
	if err != nil {
		log.Fatal("Ошибка при обработке событий: ", err)
	}

	fmt.Println("База данных успешно заполнена событиями!")
}

// Функция для обработки событий
func processEvents(eventsDir string, db *sql.DB) error {
	// Сканируем все категории (папки в /events/)
	categories, err := ioutil.ReadDir(eventsDir)
	if err != nil {
		return fmt.Errorf("ошибка при сканировании каталога %s: %v", eventsDir, err)
	}

	// Обрабатываем каждую категорию
	for _, category := range categories {
		if !category.IsDir() {
			continue
		}

		categoryName := category.Name()

		// Добавление категории в базу данных
		categoryID, err := addCategory(categoryName, db)
		if err != nil {
			return err
		}

		// Путь к папке категории
		categoryPath := filepath.Join(eventsDir, categoryName)

		// Сканируем события в категории
		events, err := ioutil.ReadDir(categoryPath)
		if err != nil {
			return fmt.Errorf("ошибка при сканировании каталога %s: %v", categoryPath, err)
		}

		// Обрабатываем каждое событие
		for _, event := range events {
			if !event.IsDir() {
				continue
			}

			eventName := event.Name()

			// Добавление события в базу данных
			eventID, err := addEvent(eventName, categoryID, db)
			if err != nil {
				return err
			}

			fmt.Printf("Добавлено событие: %s\n", eventName)

			formatPath := filepath.Join(categoryPath, eventName, "format")
			formatBytes, err := ioutil.ReadFile(formatPath)
			if err != nil {
				// Просто пропускаем, если format не существует
				continue
			}

			// Сохраняем структуру
			err = addStructure(eventID, eventName, string(formatBytes), db)
			if err != nil {
				return err
			}

		}
	}

	return nil
}

// Функция для добавления категории в базу данных
func addCategory(name string, db *sql.DB) (int, error) {
	var categoryID int
	err := db.QueryRow("SELECT id FROM categories WHERE name = $1", name).Scan(&categoryID)

	// Если категория не найдена, добавляем её
	if err == sql.ErrNoRows {
		err = db.QueryRow("INSERT INTO categories(name) VALUES($1) RETURNING id", name).Scan(&categoryID)
		if err != nil {
			return 0, fmt.Errorf("не удалось добавить категорию %s: %v", name, err)
		}
	} else if err != nil {
		return 0, fmt.Errorf("не удалось проверить категорию %s: %v", name, err)
	}

	return categoryID, nil
}

// Функция для добавления события в базу данных
func addEvent(name string, categoryID int, db *sql.DB) (int, error) {
	var eventID int
	err := db.QueryRow("SELECT id FROM events WHERE name = $1 AND category_id = $2", name, categoryID).Scan(&eventID)

	// Если событие не найдено, добавляем его
	if err == sql.ErrNoRows {
		err = db.QueryRow("INSERT INTO events(name, category_id) VALUES($1, $2) RETURNING id", name, categoryID).Scan(&eventID)
		if err != nil {
			return 0, fmt.Errorf("не удалось добавить событие %s: %v", name, err)
		}
	} else if err != nil {
		return 0, fmt.Errorf("не удалось проверить событие %s: %v", name, err)
	}

	return eventID, nil
}

func addStructure(eventID int, name string, format string, db *sql.DB) error {
	var id int
	err := db.QueryRow(
		"SELECT id FROM structures WHERE event_id = $1 AND name = $2",
		eventID, name,
	).Scan(&id)

	if err == sql.ErrNoRows {
		_, err := db.Exec(
			"INSERT INTO structures(event_id, name, format) VALUES($1, $2, $3)",
			eventID, name, format,
		)
		if err != nil {
			return fmt.Errorf("не удалось добавить структуру для события %s: %v", name, err)
		}
	} else if err != nil {
		return fmt.Errorf("не удалось проверить структуру события %s: %v", name, err)
	}

	return nil
}


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

CREATE TABLE structures (
    id SERIAL PRIMARY KEY,
    event_id INTEGER REFERENCES events(id) ON DELETE CASCADE,
    name TEXT,
    format TEXT
);



formatPath := filepath.Join(categoryPath, eventName, "format")
fmt.Printf("Проверка файла: %s\n", formatPath)

formatBytes, err := ioutil.ReadFile(formatPath)
if err != nil {
	fmt.Printf("❌ Не удалось прочитать формат для %s/%s: %v\n", categoryName, eventName, err)
	continue
}

formatStr := string(formatBytes)
if len(formatStr) == 0 {
	fmt.Printf("⚠️  Файл format для %s/%s пуст\n", categoryName, eventName)
	continue
}

// Выводим содержимое структуры в консоль
fmt.Printf("📄 Структура события %s/%s:\n%s\n", categoryName, eventName, formatStr)

err = addStructure(eventID, eventName, formatStr, db)
if err != nil {
	fmt.Printf("‼️ Ошибка сохранения структуры для %s: %v\n", eventName, err)
}






Добавлено событие: sys_enter_exit
Проверка файла: /sys/kernel/debug/tracing/events/syscalls/sys_enter_exit/format
📄 Структура события syscalls/sys_enter_exit:
name: sys_enter_exit
ID: 147
format:
        field:unsigned short common_type;       offset:0;       size:2; signed:0;
        field:unsigned char common_flags;       offset:2;       size:1; signed:0;
        field:unsigned char common_preempt_count;       offset:3;       size:1; signed:0;
        field:int common_pid;   offset:4;       size:4; signed:1;

        field:int __syscall_nr; offset:8;       size:4; signed:1;
        field:int error_code;   offset:16;      size:8; signed:0;

print fmt: "error_code: 0x%08lx", ((unsigned long)(REC->error_code))


