# eBPF verifier checks

## Проверенные ядра

Объект `bpf_x86_bpfel.o` проверен на совместимость с BTF следующих Ubuntu
generic-ядер `x86_64`:

| Ядро | CO-RE relocation | Программы приняты verifier | Максимум инструкций |
|---|---:|---:|---:|
| `4.15.0-213-generic` | PASS | 26/26 | 73,338 |
| `4.18.0-25-generic` | PASS | 26/26 | 73,338 |
| `5.0.0-65-generic` | PASS | 26/26 | 73,338 |
| `5.3.0-76-generic` | PASS | 26/26 | 73,338 |
| `5.4.0-91-generic` | PASS | 26/26 | 73,338 |
| `5.8.0-63-generic` | PASS | 26/26 | 73,338 |

Для каждой версии выполнена CO-RE-релокация по BTF целевого ядра. После
релокации все 26 eBPF-программ успешно загружены в verifier активного ядра
`6.8.0-134-generic`. Самая сложная программа — `trace_sendmmsg_exit`: 73,338
обработанных инструкций при лимите 1,000,000.

Эта матрица подтверждает совместимость структур BTF и CO-RE-релокаций. Она не
подтверждает полноценную runtime-работу tracepoint'ов на каждом старом ядре:
для этого соответствующее ядро необходимо загрузить в VM или на отдельной
машине, прикрепить все программы и выполнить интеграционный сетевой тест.

Подробный отчёт и verifier-логи находятся в
`verifier-results/2026-07-11-old-kernels/`.

Build the checker and validate every program against the running kernel:

```sh
go build -o verifier-check ./cmd/verifier
sudo ./verifier-check \
  -object bpf_x86_bpfel.o \
  -log-dir verifier-logs/$(uname -r)
```

The command exits non-zero if ELF parsing, CO-RE relocation, map creation, or
kernel verification fails. A verifier log is written for every program,
including instruction and state statistics.

## Checking CO-RE against another installed Ubuntu kernel

Extract `vmlinux` and its raw BTF section:

```sh
sudo /usr/src/linux-headers-<version>/scripts/extract-vmlinux \
  /boot/vmlinuz-<version> > /tmp/vmlinux-<version>
llvm-objcopy --dump-section .BTF=/tmp/vmlinux-<version>.btf \
  /tmp/vmlinux-<version>
sudo ./verifier-check \
  -object bpf_x86_bpfel.o \
  -kernel-btf /tmp/vmlinux-<version>.btf \
  -log-dir verifier-logs/<version>-core
```

This proves that CO-RE relocations work with the target kernel BTF and that the
relocated programs pass the running kernel's verifier. Full compatibility still
requires booting the target kernel (or a VM using it), attaching all tracepoints,
and exercising the network paths.
