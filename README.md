

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

link, err := link.RawAttachProgram(link.RawAttachProgramOptions{
		Program: objs.EchoDispatch,
		Attach:  ebpf.AttachSkLookup,
	})

	if err != nil {
		log.Fatalf("failed to attach sk_lookup: %v", err)
	}
	defer link.Close()


[{
	"resource": "/home/gaz358/myprog/bpfgo/main.go",
	"owner": "_generated_diagnostic_collection_name_#0",
	"code": {
		"value": "WrongAssignCount",
		"target": {
			"$mid": 1,
			"path": "/golang.org/x/tools/internal/typesinternal",
			"scheme": "https",
			"authority": "pkg.go.dev",
			"fragment": "WrongAssignCount"
		}
	},
	"severity": 8,
	"message": "assignment mismatch: 2 variables but link.RawAttachProgram returns 1 value",
	"source": "compiler",
	"startLineNumber": 44,
	"startColumn": 15,
	"endLineNumber": 47,
	"endColumn": 4
}]

func RawAttachProgram(opts RawAttachProgramOptions) error {
	if opts.Flags&anchorFlags != 0 {
		return fmt.Errorf("disallowed flags: use Anchor to specify attach target")
	}

	attr := sys.ProgAttachAttr{
		TargetFdOrIfindex: uint32(opts.Target),
		AttachBpfFd:       uint32(opts.Program.FD()),
		AttachType:        uint32(opts.Attach),
		AttachFlags:       uint32(opts.Flags),
		ExpectedRevision:  opts.ExpectedRevision,
	}

	if opts.Anchor != nil {
		fdOrID, flags, err := opts.Anchor.anchor()
		if err != nil {
			return fmt.Errorf("attach program: %w", err)
		}

		if flags == sys.BPF_F_REPLACE {
			// Ensure that replacing a program works on old kernels.
			attr.ReplaceBpfFd = fdOrID
		} else {
			attr.RelativeFdOrId = fdOrID
			attr.AttachFlags |= flags
		}
	}

	if err := sys.ProgAttach(&attr); err != nil {
		if haveFeatErr := haveProgAttach(); haveFeatErr != nil {
			return haveFeatErr
		}
		return fmt.Errorf("attach program: %w", err)
	}
	runtime.KeepAlive(opts.Program)

	return nil
}















