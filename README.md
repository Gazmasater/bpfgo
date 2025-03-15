

nc -l 12345

nc 127.0.0.1 12345



bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

Как загрузить eBPF-программу с BPF_SK_LOOKUP и привязать её к network namespace
1. Скомпилировать eBPF-программу
Если твоя программа находится в test.c, скомпилируй её с помощью clang:


clang -O2 -g -target bpf -c test.c -o test.o
2. Создать bpffs (если его нет)
Чтобы eBPF-программы могли загружаться, убедись, что bpffs смонтирован:

sudo mount -t bpf none /sys/fs/bpf
Если он уже смонтирован, этот шаг можно пропустить.

3. Создать новое network namespace (если нужно)
Если хочешь тестировать программу в изолированном пространстве имен сети, создай его:


sudo ip netns add my_netns
Проверить список network namespace можно так:


ip netns list
4. Загрузить eBPF-программу в ядро
Используем bpftool:


sudo bpftool prog load test.o /sys/fs/bpf/test_prog type sk_lookup
Это загружает eBPF-программу в ядро и сохраняет её в bpffs.

Проверить, загружена ли программа:


bpftool prog show
Если sk_lookup присутствует, значит загрузка прошла успешно.

5. Привязать программу к network namespace
Чтобы eBPF-программа работала, её нужно прикрепить к сетевому пространству:


sudo bpftool net attach sk_lookup id $(bpftool prog show | grep sk_lookup | awk '{print $1}') netns /var/run/netns/my_netns
Где:

$(bpftool prog show | grep sk_lookup | awk '{print $1}') — получает ID загруженной eBPF-программы.
/var/run/netns/my_netns — путь к пространству имен сети (указывается вместо target_fd).
Проверить, что программа прикреплена:


6. Проверить работу
Теперь можно выполнить команду внутри network namespace, чтобы проверить, что программа работает:


sudo ip netns exec my_netns ping -c 3 8.8.8.8
Если eBPF-программа корректно перехватывает трафик, ping может не пройти или показать задержку.


package main

import (
	"fmt"
	"log"
	"os"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/link"
)

func main() {
	// Загружаем eBPF-программу
	spec, err := ebpf.LoadCollectionSpec("test.bpf.o")
	if err != nil {
		log.Fatalf("Ошибка загрузки eBPF-программы: %v", err)
	}

	// Создаём eBPF Collection
	objs := struct {
		SkLookup *ebpf.Program `ebpf:"sk_lookup_prog"`
	}{}
	if err := spec.LoadAndAssign(&objs, nil); err != nil {
		log.Fatalf("Ошибка загрузки eBPF-программы в ядро: %v", err)
	}
	defer objs.SkLookup.Close()

	// Открываем файловый дескриптор текущего network namespace
	netns, err := os.Open("/proc/self/ns/net")
	if err != nil {
		log.Fatalf("Ошибка открытия текущего network namespace: %v", err)
	}
	defer netns.Close()

	// Привязываем eBPF-программу к текущему namespace
	lnk, err := link.AttachNetNs(link.NetNsOptions{
		Program: objs.SkLookup,
		NetNS:   netns,
	})
	if err != nil {
		log.Fatalf("Ошибка привязки sk_lookup к namespace: %v", err)
	}
	defer lnk.Close()

	fmt.Println("eBPF программа успешно загружена и привязана к текущему namespace!")

	select {} // Бесконечный цикл
}

SEC("sk_lookup")
int echo_dispatch(struct bpf_sk_lookup *ctx)
{
    __u32 pid = bpf_get_current_pid_tgid() >> 32;
    __u8 proto = ctx->protocol; // Протокол

    // Выводим значение протокола в лог
    bpf_printk("PID: %d, Protocol Number: %d\n", pid, proto);

    // Определяем протокол (TCP или UDP)
    const char *proto_str;
    if (proto == IPPROTO_TCP) {
        proto_str = "TCP";
    } else if (proto == IPPROTO_UDP) {
        proto_str = "UDP";
    } else {
        proto_str = "UNKNOWN";
    }

    bpf_printk("PID: %d, Protocol: %s\n", pid, proto_str);
    return SK_PASS;
}

err := link.RawAttachProgram(link.RawAttachProgramOptions{
		Program: objs.EchoDispatch,
		Attach:  ebpf.AttachSkLookup,
	})

	if err != nil {
		log.Fatalf("failed to attach sk_lookup: %v", err)
	}

Получите идентификатор программы:

bpftool prog show | grep sk_lookup | awk '{print $1}'
Получите индекс интерфейса внутри вашего сетевого пространства:

ip netns exec my_netns ip link
Затем выполните команду с полными параметрами, например:

sudo bpftool net attach sk_lookup id <prog_id> netns /var/run/netns/my_netns ifindex <ifindex>


Шаг 1: Добавьте класс управления трафиком для интерфейса lo (если он еще не был добавлен)
Это необходимо, чтобы можно было добавлять фильтры на интерфейс:


sudo tc qdisc add dev lo clsact
Шаг 2: Прикрепите программу BPF для входящего трафика
Это действие уже описано:


sudo tc filter add dev lo ingress bpf obj ./sk_lookup.o sec sk_lookup
Шаг 3: Прикрепите программу BPF для исходящего трафика
Теперь нужно добавить фильтр для исходящего трафика, используя egress:


sudo tc filter add dev lo egress bpf obj ./sk_up.o sec sk_lookup
dev lo — указывает на интерфейс lo.
egress — указывает, что фильтр будет работать с исходящим трафиком.
bpf obj ./sk_lookup.o — указывает на объектный файл программы BPF.
sec sk_lookup — указывает на секцию в объектном файле программы.
Шаг 4: Проверка фильтров на интерфейсе lo
Чтобы убедиться, что фильтры были успешно добавлены для обоих направлений трафика, выполните команду:


sudo tc -s filter show dev lo
Эта команда покажет статистику для обоих фильтров (входящего и исходящего трафика) на интерфейсе lo.

sudo bpftool prog delete id <ID программы>

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo/bpf$ sudo bpftool prog delete id 66
Usage: bpftool prog { show | list } [PROG]
       bpftool prog dump xlated PROG [{ file FILE | [opcodes] [linum] [visual] }]
       bpftool prog dump jited  PROG [{ file FILE | [opcodes] [linum] }]
       bpftool prog pin   PROG FILE
       bpftool prog { load | loadall } OBJ  PATH \
                         [type TYPE] [{ offload_dev | xdpmeta_dev } NAME] \
                         [map { idx IDX | name NAME } MAP]\
                         [pinmaps MAP_DIR]
                         [autoattach]
       bpftool prog attach PROG ATTACH_TYPE [MAP]
       bpftool prog detach PROG ATTACH_TYPE [MAP]
       bpftool prog run PROG \
                         data_in FILE \
                         [data_out FILE [data_size_out L]] \
                         [ctx_in FILE [ctx_out FILE [ctx_size_out M]]] \
                         [repeat N]
       bpftool prog profile PROG [duration DURATION] METRICs
       bpftool prog tracelog
       bpftool prog help

       MAP := { id MAP_ID | pinned FILE | name MAP_NAME }
       PROG := { id PROG_ID | pinned FILE | tag PROG_TAG | name PROG_NAME }
       TYPE := { socket | kprobe | kretprobe | classifier | action |
                 tracepoint | raw_tracepoint | xdp | perf_event | cgroup/skb |
                 cgroup/sock | cgroup/dev | lwt_in | lwt_out | lwt_xmit |
                 lwt_seg6local | sockops | sk_skb | sk_msg | lirc_mode2 |
                 sk_reuseport | flow_dissector | cgroup/sysctl |
                 cgroup/bind4 | cgroup/bind6 | cgroup/post_bind4 |
                 cgroup/post_bind6 | cgroup/connect4 | cgroup/connect6 |
                 cgroup/connect_unix | cgroup/getpeername4 | cgroup/getpeername6 |
                 cgroup/getpeername_unix | cgroup/getsockname4 | cgroup/getsockname6 |
                 cgroup/getsockname_unix | cgroup/sendmsg4 | cgroup/sendmsg6 |
                 cgroup/sendmsg_unix | cgroup/recvmsg4 | cgroup/recvmsg6 | cgroup/recvmsg_unix |
                 cgroup/getsockopt | cgroup/setsockopt | cgroup/sock_release |
                 struct_ops | fentry | fexit | freplace | sk_lookup }
       ATTACH_TYPE := { sk_msg_verdict | sk_skb_verdict | sk_skb_stream_verdict |
                        sk_skb_stream_parser | flow_dissector }
       METRIC := { cycles | instructions | l1d_loads | llc_misses | itlb_misses | dtlb_misses }
       OPTIONS := { {-j|--json} [{-p|--pretty}] | {-d|--debug} |
                    {-f|--bpffs} | {-m|--mapcompat} | {-n|--nomount} |
                    {-L|--use-loader} }
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo/bpf$ 























