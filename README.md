
https://platform-balun.ru/teach/control/stream/view/id/930914027
user: mat3x@mail.ru
pass: Vkalashnikov00f!



bpf2go -output-dir . -tags linux -type trace_info -go-package=main -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)



sudo nft add table ip6 test
sudo nft add chain ip6 test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule ip6 test prerouting exthdr type routing accept

chain prerouting {
    type nat hook prerouting priority 0;
}
sudo nft add rule ip test prerouting ip daddr 192.168.1.0/24 accept




sudo nft add table inet test
sudo nft add chain inet test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule inet test prerouting exthdr dst exists accept


sudo nft list ruleset

table inet test {
        chain prerouting {
                type filter hook prerouting priority filter; policy accept;
                exthdr dst exists accept
        }
}


sudo nft add rule ip6 test prerouting exthdr dst exists accept
sudo nft add rule ip6 test prerouting exthdr hopopts exists accept
sudo nft add rule ip6 test prerouting exthdr frag exists accept
sudo nft add rule ip6 test prerouting exthdr routing exists accept



git rebase -i db77c5b 
Первый из них оставляешь как pick, остальные 9 меняешь на squash (или s):

git rebase --abort


git push -u origin trace_core1 --force




_______________________________________________________________________________________________


		// Задаём batchSize
		const batchSize = 4
		records := make([]perf.Record, batchSize)

		// Размер кольца (pageCount) можно оставить таким же или увеличить при необходимости
		// (buffLen здесь – это pageCount)
		const buffLen = 4096 * 2
		rd, err := perf.NewReader(objs.TraceEvents, buffLen)
		if err != nil {
			log.Fatalf("failed to create perf reader: %s", err)
		}
		defer rd.Close()

		executableName := os.Args[0]
		if len(executableName) > 2 {
			executableName = executableName[2:]
		}

		for {
			// Читаем сразу до batchSize записей
			n, err := rd.ReadBatch(records, perf.ReadBatchOptions{})
			if err != nil {
				if errors.Is(err, os.ErrDeadlineExceeded) {
					continue
				}
				log.Fatalf("error reading batch from perf reader: %v", err)
			}

			// Проверяем, не потерялись ли какие-то события
			if lost := rd.LostSamples(); lost > 0 {
				log.Printf("WARNING: lost %d samples (ring overflow)", lost)
			}

			// Обрабатываем каждую из n прочитанных записей
			for i := 0; i < n; i++ {
				record := records[i]

				// Если реальный размер RawSample < ожидаемого — пропускаем
				if len(record.RawSample) < int(unsafe.Sizeof(bpfTraceInfo{})) {
					log.Println("!!!!!!!!!!!!!!!!!!!!invalid event size!!!!!!!!!!!!!!!!!!")
					continue
				}

				// Распарсим одно событие
				event := *(*bpfTraceInfo)(unsafe.Pointer(&record.RawSample[0]))

				srcIP := net.IPv4(
					byte(event.SrcIP.S_addr),
					byte(event.SrcIP.S_addr>>8),
					byte(event.SrcIP.S_addr>>16),
					byte(event.SrcIP.S_addr>>24),
				)
				dstIP := net.IPv4(
					byte(event.DstIP.S_addr),
					byte(event.DstIP.S_addr>>8),
					byte(event.DstIP.S_addr>>16),
					byte(event.DstIP.S_addr>>24),
				)

				if pkg.Int8ToString(event.Comm) == executableName {
					continue
				}












