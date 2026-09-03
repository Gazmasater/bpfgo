# Эксплуатация и нагрузочная проверка

## Перед запуском

1. Проверьте требования из [kernel-compatibility.md](kernel-compatibility.md).
2. Начните с закрытого pprof (`-pprof=false`, значение по умолчанию).
3. Задайте достаточный `-perfMB`; при большем числе CPU выделяемый budget делится
   между CPU, а reader выбирает степень двойки и умеет уменьшать buffer при OOM.
4. При необходимости ограничьте область `-pid` или `-comm`.

## Наблюдаемые сигналы

| Сигнал | Значение | Действие |
| --- | --- | --- |
| `PERF_LOST` | perf buffer переполнен | увеличить `-perfMB`, сузить scope, повторить тест |
| `PERF_LOST_RATE` | скорость потерь за интервал | использовать для сравнения конфигураций |
| `maybe_lost=1` | поток мог потерять события | не считать такие counters точными |
| `incomplete=1` | endpoint установлен не полностью | проверить L3/tracepoints и характер workload |
| `SNI reader disabled` | TLS enrichment недоступен | потоковая трассировка продолжает работать |

Для машинного сбора включите `-metricsAddr=127.0.0.1:9464` и читайте
`/debug/vars`. Endpoint содержит `bpfgo_perf_lost_total`,
`bpfgo_flows_active`, `bpfgo_flow_records_total` и `bpfgo_sni_extracted_total`.
Он намеренно выключен по умолчанию и обслуживает только expvar, без pprof.
Чтобы передавать сами результаты в log pipeline, используйте `-json`: каждая
строка `LIVE`/`FLOW` становится отдельным JSON document.

TLS SNI по умолчанию захватывается только на TCP:443. Для альтернативных
endpoint-ов задайте ограниченный явный список, например
`-tlsPorts=443,8443,9443`; это правило применяется внутри BPF до чтения
application payload.

## Базовый нагрузочный сценарий

Запускайте на стенде, а не на production host:

```sh
# терминал 1
sudo ./bpfgo -perfMB=32 -print=1s -resolve=false -sni=false

# терминал 2: TCP throughput (iperf3 server должен быть запущен на peer)
iperf3 -c <peer> -P 8 -t 60

# терминал 3: UDP throughput
iperf3 -u -c <peer> -b 100M -t 60
```

Повторите тест с включёнными `-l3`, `-rw`, `-mmsg`, `-resolve` и `-sni`, меняя
только один параметр за раз. Зафиксируйте CPU, RSS, число потоков и
`PERF_LOST_RATE`; pprof можно временно открыть только на loopback:

```sh
sudo ./bpfgo -pprof -pprofAddr=127.0.0.1:6060
go tool pprof http://127.0.0.1:6060/debug/pprof/profile?seconds=30
```

## Критерии готовности

- Нет `PERF_LOST` на ожидаемом пике либо задокументирован допустимый уровень
  потерь.
- Нет verifier/attach errors и не отключены требуемые probe groups.
- Количество байтов и потоков согласуется с независимым traffic generator.
- Программа корректно останавливается по SIGTERM и не сохраняет растущие cache
  после TTL sweep.
