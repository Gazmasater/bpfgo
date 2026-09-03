# Совместимость с ядрами

`bpfgo` использует CO-RE, но совместимость определяется не только наличием BTF.
Итоговую гарантию даёт загрузка и выполнение именно на target kernel.

## Поддерживаемая платформа

| Параметр | Статус |
| --- | --- |
| ОС | Linux |
| Архитектура BPF object | amd64 (`bpf_x86_bpfel`) |
| BTF | обязателен: `/sys/kernel/btf/vmlinux` |
| BPF style | tracepoint + CO-RE, perf event array |
| Привилегии | `CAP_BPF`, `CAP_PERFMON`, `CAP_SYS_RESOURCE` |

## Исторический результат verifier-проверки

В проектной истории зафиксирована успешная CO-RE relocation объекта для Ubuntu
generic BTF: `4.15.0-213`, `4.18.0-25`, `5.0.0-65`, `5.3.0-76`, `5.4.0-91` и
`5.8.0-63` (`x86_64`). Проверка verifier выполнялась на ядре `6.8.0-134`; это
не является runtime-подтверждением перечисленных старых ядер.

## Release gate для новой kernel/BTF комбинации

1. Убедиться, что доступны BTF и все нужные tracepoints:

   ```sh
   test -r /sys/kernel/btf/vmlinux
   test -e /sys/kernel/tracing/events/syscalls/sys_enter_connect
   test -e /sys/kernel/tracing/events/net/net_dev_queue
   ```

2. Собрать bindings и выполнить быстрые тесты:

   ```sh
   go generate ./...
   go test ./...
   ```

3. На изолированной машине запустить integration suite и проверить, что BPF
   objects загружаются без verifier errors:

   ```sh
   sudo BPFGO_INTEGRATION=1 go test -count=1 ./...
   ```

4. Прогнать workload из [operations.md](operations.md), убедиться в отсутствии
   `PERF_LOST`, и записать kernel release, BTF source, дистрибутив и результат
   в release notes.

## Важные ограничения

- Объект генерируется только для `amd64`; ARM64 требует отдельной генерации и
  проверки.
- Необязательные группы (`mmsg`, `rw`, `l3`) отключаются целиком, если один
  tracepoint не прикрепился. Отсутствие обязательной группы останавливает старт.
- Поведение tracepoints, CO-RE layout и доступные capabilities могут отличаться
  в VM, container и hardened distribution.
