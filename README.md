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


STATE=3 DST IPv6=ffff9c71:427e60b0:ffff9c71:0
STATE=3 SRC IPv6=0:ac5e9400:1828deb4:ac17e8c0
STATE=3 SPORT=33333  DPORT=33812

Server IPv6

//go:build ignore

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 33333
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in6 server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(client_addr);

    // Создаем сокет
    if ((sockfd = socket(AF_INET6, SOCK_DGRAM, 0)) < 0) {
        perror("Ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }

    // Задаем параметры адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = AF_INET6;
    inet_pton(AF_INET6, "::1", &server_addr.sin6_addr); // Используем ::1 для локального адреса
    server_addr.sin6_port = htons(PORT);

    // Привязываем сокет
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Ошибка при привязке сокета");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP-сервер (IPv6) запущен и слушает [::1]:%d...\n", PORT);

    // Принимаем сообщения
    while (1) {
        int len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, 
                           (struct sockaddr*)&client_addr, &addr_len);
        if (len < 0) {
            perror("Ошибка при получении данных");
            continue;
        }
        buffer[len] = '\0';  // Завершаем строку
        printf("Получено сообщение: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}

Client IPv6

//go:build ignore

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 33333
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in6 server_addr;
    char buffer[BUFFER_SIZE] = "Привет, сервер!";
    
    // Создаем сокет
    if ((sockfd = socket(AF_INET6, SOCK_DGRAM, 0)) < 0) {
        perror("Ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }

    // Задаем параметры адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = AF_INET6;
    inet_pton(AF_INET6, "::1", &server_addr.sin6_addr); // Локальный IPv6 адрес
    server_addr.sin6_port = htons(PORT);

    // Отправляем сообщение
    if (sendto(sockfd, buffer, strlen(buffer), 0, 
               (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Ошибка при отправке данных");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Сообщение отправлено на [::1]:%d: %s\n", PORT, buffer);

    close(sockfd);
    return 0;
}


