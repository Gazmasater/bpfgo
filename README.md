# bpfgo

`bpfgo` — Linux/x86_64 eBPF-трассировщик сетевых потоков. Он получает события
socket syscalls и `net_dev_queue`, объединяет их в TCP/UDP/ICMP потоки и выводит
адреса, объём трафика, число пакетов, PTR-имена и TLS SNI (best effort).

## Возможности

- IPv4 и IPv6; TCP, UDP, ICMP и ICMPv6.
- Трассировка `bind`, `connect`, `accept`, `close`, `sendto`/`recvfrom`,
  `sendmsg`/`recvmsg`; опционально `sendmmsg`/`recvmmsg` и socket `read`/`write`.
- Корреляция событий по inode socket-а (`cookie`). Для UDP и ICMP удалённый peer
  входит в ключ, поэтому один сокет может содержать несколько потоков.
- Правильная обработка `msg_name`, `IP_PKTINFO`/`IPV6_PKTINFO` и неприкреплённых
  UDP-сокетов; receive с `MSG_PEEK` не учитываются.
- L3-подсказки с `net_dev_queue`, чтобы заполнить фактически выбранный source IP.
- Асинхронный обратный DNS с cache и необязательным prefill из `/etc/hosts`.
- Извлечение SNI из TLS ClientHello на исходящем TCP с настраиваемыми портами
  (по умолчанию `443`). После успеха BPF прекращает захват payload для socket-а.

## Требования

- Linux, `amd64`; для загрузки нужен BTF (`/sys/kernel/btf/vmlinux`).
- Go 1.24.2 или новее для сборки userspace.
- `clang` и `llvm-strip` для `go generate`.
- Привилегии `CAP_BPF`, `CAP_PERFMON`, `CAP_SYS_RESOURCE` (либо эквивалентный
  root-доступ в подходящей конфигурации ядра).

Программа использует CO-RE, но это не делает её автоматически переносимой на
каждое ядро: перед развёртыванием обязательно запускайте integration-тесты на
целевой версии ядра и дистрибутиве.

## Сборка и запуск

```sh
go generate ./...
go test ./...
go build -o bpfgo .
sudo ./bpfgo
```

Для systemd есть пример [deploy/bpfgo-tracer.service](deploy/bpfgo-tracer.service).
Перед его использованием замените пути `WorkingDirectory` и `ExecStart`.

Пример фильтрации одного процесса:

```sh
sudo ./bpfgo -pid 1234 -sni=true -l3=true
```

## Вывод

При появлении готового потока печатается `LIVE`, а при `close`, idle TTL или
остановке — финальная строка `FLOW`:

```text
FLOW  TCP   pid=1234(curl) cookie=…  192.0.2.10:41000 <-> 198.51.100.20:443  out=… in=… age=… end=close() sni=example.com
```

Метка `incomplete=1` означает, что не все endpoint-поля удалось получить;
`maybe_lost=1` сигнализирует о потерях в perf buffer.

## Конфигурация

Все флаги доступны и как переменные окружения с префиксом `BPFGO_`; например,
`BPFGO_PERF_MB=16` соответствует `-perfMB=16`. При старте автоматически
загружается ближайший `.env`.

| Группа | Основные флаги |
| --- | --- |
| Потоки | `-ttl`, `-print`, `-pid`, `-comm`, `-perfMB` |
| Пробы | `-rw`, `-mmsg`, `-l3`, `-l3ttl`, `-openDelay` |
| DNS | `-resolve`, `-resolveTTL`, `-resolveWorkers`, `-hostsPrefill` |
| TLS | `-sni`, `-tlsPorts`, `-sniTTL`, `-sniMax` |
| Диагностика | `-pprof`, `-pprofAddr`, `-metricsAddr`, `-json` |

`-sni=false` отключает не только parser в Go, но и чтение BPF user payload.
`-tlsPorts=443,8443,9443` задаёт до 64 TCP-портов для захвата SNI; BPF не читает
payload TCP-соединений на остальных портах. Эквивалентная переменная окружения —
`BPFGO_TLS_PORTS`.
`-pprof` по умолчанию выключен; не публикуйте его на внешнем интерфейсе.
`-json` меняет только вывод потоков на JSON Lines. `-metricsAddr=127.0.0.1:9464`
включает JSON endpoint `/debug/vars`; он выключен по умолчанию и не запускает pprof.

## Архитектура

```text
tracepoints → BPF maps / CO-RE socket state → perf events
            → Go readers → flow cache → LIVE / FLOW
```

- `internal/ebpf/trace.bpf.c` — единственная BPF translation unit.
- `internal/ebpf/abi.h`, `maps.h`, `helpers.h` — ABI, maps и общие helpers.
- `internal/ebpf/probes/` — handlers для socket, I/O, lifecycle и L3 событий.
- `main.go` — загрузка объектов, attachment, event loop.
- `flow.go`, `resolver.go`, `tls_sni.go` — агрегация, PTR cache и ClientHello parser.

Go bindings и BPF object создаёт `bpf2go` из директивы в `main.go`. Не редактируйте
`bpf_x86_bpfel.go` вручную.

## Тестирование

```sh
# Быстрые unit-тесты
go test ./...

# Linux/eBPF integration-тесты (нужны привилегии и поддерживаемое ядро)
sudo BPFGO_INTEGRATION=1 go test -tags=integration -count=1 ./...
```

Integration suite проверяет UDP, TCP loopback, TLS SNI, feature flags и PID
filter. CI проверяет formatting, unit-тесты и актуальность bpf2go-generated
bindings; integration-тесты должны выполняться на kernel matrix перед релизом.

## Эксплуатационные ограничения

- SNI — best effort: только ClientHello, только outgoing TCP на портах из
  `-tlsPorts`,
  максимум 512 B на BPF chunk и 16 KiB reassembly на соединение.
- `net_dev_queue` — подсказка, а не единственный источник истины для endpoint.
- При перегрузке perf могут теряться события: увеличьте `-perfMB`, отследите
  `PERF_LOST` и проведите нагрузочную проверку из
  [docs/operations.md](docs/operations.md).
- Неподдерживаемый tracepoint отключает только необязательную группу probes;
  обязательные socket probes останавливают запуск с ошибкой.

## Разработка

```sh
gofmt -w *.go pkg/*.go
go generate ./...
go test ./...
git diff --check
```

Обновляйте [docs/kernel-compatibility.md](docs/kernel-compatibility.md) после
проверки новой комбинации kernel/BTF/дистрибутива.
