
https://platform-balun.ru/teach/control/stream/view/id/930914027
user: mat3x@mail.ru
pass: Vkalashnikov00f!

__________________________________________________
GIT

git reset --hard HEAD
____________________________________________________



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


go tool pprof http://localhost:6060/debug/pprof/profile?seconds=30


"net/http"      // добавлено для pprof
	_ "net/http/pprof" // регистрирует pprof-эндпоинты

     _ "net/http/pprof" // blank-import: регистрирует pprof-хэндлеры в DefaultServeMux



package main

import (
	"errors"
	"fmt"
	"log"
	"net"
	"net/http"
	_ "net/http/pprof"
	"os"
	"os/signal"
	"sync"
	"syscall"
	"unsafe"
	"time"

	"bpfgo/pkg"

	"github.com/cilium/ebpf/link"
	"github.com/cilium/ebpf/perf"
	"github.com/cilium/ebpf/rlimit"
)

// bpfObjects содержит загруженные eBPF-объекты
var bpfObjects bpfObjects

// Создаем два канала для передачи портов и PID между состояниями
var portStateChannel = make(chan int, 1)
var processIDChannel = make(chan int, 1)

// Мьютекс для синхронизации доступа к каналам
var channelMutex sync.Mutex

// Хранит последнюю прочитанную ступень протокола и соответствующие хосты
var protocolString, destinationHost, sourceHost string

// Синх-пул для переиспользования структур EventData
var eventDataPool = sync.Pool{
	New: func() interface{} {
		return new(EventData)
	},
}

// Кэш для DNS: хранит соответствие IP → имя хоста и время последнего обновления
var dnsCache = struct {
	sync.RWMutex
	entries map[string]cachedDNSRecord
}{
	entries: make(map[string]cachedDNSRecord),
}

// cachedDNSRecord — запись кэша DNS с данными и временем устаревания
type cachedDNSRecord struct {
	hostName    string
	lastUpdated time.Time
}

// Как долго считаем запись кэшированной (например, 5 минут)
const dnsCacheTTL = 5 * time.Minute

// Структуры для хранения данных сетевых событий
type LookupEvent struct {
	DestinationIP   net.IP
	DestinationPort int
	SourceIP        net.IP
	SourcePort      int
	ProtocolNumber  int
}

type SendmsgEvent struct {
	DestinationIP   net.IP
	DestinationPort int
	ProcessID       uint32
	ProtocolNumber  int
	CommandName     string
}

type RecvmsgEvent struct {
	SourceIP        net.IP
	SourcePort      int
	ProcessID       uint32
	ProtocolNumber  int
	CommandName     string
}

// EventData группирует все данные, поступившие от BPF-программ для одного потока
type EventData struct {
	LookupInfo     LookupEvent
	SendMessage    SendmsgEvent
	ReceiveMessage RecvmsgEvent
	HasLookup      bool
	HasSendMessage bool
	HasReceive     bool
}

func init() {
	// Снимаем ограничение на использование памяти для eBPF
	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("Не удалось убрать лимит по памяти для eBPF: %v", err)
	}
	// Загружаем eBPF-объекты
	if err := loadBpfObjects(&bpfObjects, nil); err != nil {
		log.Fatalf("Не удалось загрузить BPF-объекты: %v", err)
	}
}

func main() {
	// Запускаем сервер pprof на порту 6060 для профилирования
	go func() {
		log.Println("Запуск pprof-сервера на :6060")
		if err := http.ListenAndServe(":6060", nil); err != nil {
			log.Fatalf("pprof ListenAndServe error: %v", err)
		}
	}()

	// eventMap хранит промежуточные данные для IPv4-событий
	eventMapIPv4 := make(map[int]*EventData)
	// eventMapIPv4Reverse хранит промежуточные данные для обработки «sendto»
	eventMapIPv4Reverse := make(map[int]*EventData)

	// Закрываем eBPF-объекты при выходе
	defer bpfObjects.Close()

	// Открываем дескриптор сетевого неймспейса
	networkNamespace, err := os.Open("/proc/self/ns/net")
	if err != nil {
		panic(err)
	}
	defer networkNamespace.Close()

	fmt.Printf("Дескриптор неймспейса: %d\n", networkNamespace.Fd())
	fmt.Printf("Размер структуры bpfTraceInfo в Go: %d байт\n", unsafe.Sizeof(bpfTraceInfo{}))

	// Привязываем tracepoint-ы к eBPF-программам
	sendmsgEnterLink, err := link.Tracepoint("syscalls", "sys_enter_sendmsg", bpfObjects.TraceSendmsgEnter, nil)
	if err != nil {
		log.Fatalf("Не удалось открыть tracepoint sys_enter_sendmsg: %s", err)
	}
	defer sendmsgEnterLink.Close()

	sendmsgExitLink, err := link.Tracepoint("syscalls", "sys_exit_sendmsg", bpfObjects.TraceSendmsgExit, nil)
	if err != nil {
		log.Fatalf("Не удалось открыть tracepoint sys_exit_sendmsg: %s", err)
	}
	defer sendmsgExitLink.Close()

	sendtoEnterLink, err := link.Tracepoint("syscalls", "sys_enter_sendto", bpfObjects.TraceSendtoEnter, nil)
	if err != nil {
		log.Fatalf("Не удалось открыть tracepoint sys_enter_sendto: %s", err)
	}
	defer sendtoEnterLink.Close()

	sendtoExitLink, err := link.Tracepoint("syscalls", "sys_exit_sendto", bpfObjects.TraceSendtoExit, nil)
	if err != nil {
		log.Fatalf("Не удалось открыть tracepoint sys_exit_sendto: %s", err)
	}
	defer sendtoExitLink.Close()

	recvmsgEnterLink, err := link.Tracepoint("syscalls", "sys_enter_recvmsg", bpfObjects.TraceRecvmsgEnter, nil)
	if err != nil {
		log.Fatalf("Не удалось открыть tracepoint sys_enter_recvmsg: %s", err)
	}
	defer recvmsgEnterLink.Close()

	recvmsgExitLink, err := link.Tracepoint("syscalls", "sys_exit_recvmsg", bpfObjects.TraceRecvmsgExit, nil)
	if err != nil {
		log.Fatalf("Не удалось открыть tracepoint sys_exit_recvmsg: %s", err)
	}
	defer recvmsgExitLink.Close()

	recvfromEnterLink, err := link.Tracepoint("syscalls", "sys_enter_recvfrom", bpfObjects.TraceRecvfromEnter, nil)
	if err != nil {
		log.Fatalf("Не удалось открыть tracepoint sys_enter_recvfrom: %s", err)
	}
	defer recvfromEnterLink.Close()

	recvfromExitLink, err := link.Tracepoint("syscalls", "sys_exit_recvfrom", bpfObjects.TraceRecvfromExit, nil)
	if err != nil {
		log.Fatalf("Не удалось открыть tracepoint sys_exit_recvfrom: %s", err)
	}
	defer recvfromExitLink.Close()

	tcpStateChangeLink, err := link.Tracepoint("sock", "inet_sock_set_state", bpfObjects.TraceTcpEst, nil)
	if err != nil {
		log.Fatalf("Не удалось открыть tracepoint inet_sock_set_state: %s", err)
	}
	defer tcpStateChangeLink.Close()

	// Привязываем программу sk_lookup к сетевому неймспейсу
	skLookupAttachment, err := link.AttachNetNs(int(networkNamespace.Fd()), bpfObjects.LookUp)
	if err != nil {
		log.Fatalf("Не удалось прикрепить программу sk_lookup: %v", err)
	}
	defer skLookupAttachment.Close()

	// Канал для корректного завершения программы по сигналу
	stopSignalChannel := make(chan os.Signal, 1)
	signal.Notify(stopSignalChannel, os.Interrupt, syscall.SIGTERM)

	// Запускаем горутину для чтения событий из perf-файла
	go func() {
		const bufferLength = 4096 * 2
		perfReader, err := perf.NewReader(bpfObjects.TraceEvents, bufferLength)
		if err != nil {
			log.Fatalf("Не удалось создать perf-ридер: %s", err)
		}
		defer perfReader.Close()

		// Получаем имя исполняемого файла (без «./» при запуске из текущей директории)
		executableName := os.Args[0]
		if len(executableName) > 2 && executableName[:2] == "./" {
			executableName = executableName[2:]
		}

		const batchSize = 4

		for {
			var batchRecords []perf.Record
			for i := 0; i < batchSize; i++ {
				record, err := perfReader.Read()
				if err != nil {
					if errors.Is(err, os.ErrDeadlineExceeded) {
						break
					}
					log.Fatalf("Ошибка при чтении из perf-ридера: %v", err)
				}
				batchRecords = append(batchRecords, record)
				if len(batchRecords) >= batchSize {
					break
				}
			}

			if len(batchRecords) == 0 {
				continue
			}

			for _, record := range batchRecords {
				// Удостоверяемся, что в записи есть полный bpfTraceInfo
				if len(record.RawSample) < int(unsafe.Sizeof(bpfTraceInfo{})) {
					log.Println("Некорректный размер события, пропускаем")
					continue
				}

				// Читаем структуру bpfTraceInfo из сырых байт
				event := *(*bpfTraceInfo)(unsafe.Pointer(&record.RawSample[0]))

				// Восстанавливаем адреса IPv4 из «host byte order»
				sourceIPv4 := net.IPv4(
					byte(event.SrcIP.S_addr),
					byte(event.SrcIP.S_addr>>8),
					byte(event.SrcIP.S_addr>>16),
					byte(event.SrcIP.S_addr>>24),
				)
				destinationIPv4 := net.IPv4(
					byte(event.DstIP.S_addr),
					byte(event.DstIP.S_addr>>8),
					byte(event.DstIP.S_addr>>16),
					byte(event.DstIP.S_addr>>24),
				)

				// Если имя процесса совпадает с именем нашего исполняемого файла, пропускаем событие
				if pkg.Int8ToString(event.Comm) == executableName {
					continue
				}

				// Обработка IPv4-событий: Sysexit == 1 (sys_exit_sendto или sendmsg)
				if event.Sysexit == 1 {
					family := event.Family
					if family == 2 {
						port := int(event.Dport)
						data, exists := eventMapIPv4Reverse[port]
						if !exists {
							// Получаем EventData из пула и обнуляем его
							data = eventDataPool.Get().(*EventData)
							*data = EventData{}
							eventMapIPv4Reverse[port] = data
						}
						data.SendMessage = SendmsgEvent{
							DestinationIP:   destinationIPv4,
							DestinationPort: port,
							ProcessID:       event.Pid,
							CommandName:     pkg.Int8ToString(event.Comm),
							ProtocolNumber:  int(event.Proto),
						}
						data.HasSendMessage = true

						// Если у нас есть информация и о Lookup, и о Recvmsg — печатаем
						if data.HasLookup && data.HasReceive {
							conversationProtocol := determineProtocolName(data.LookupInfo.ProtocolNumber)
							sourceHostName := resolveHostnameWithCache(data.LookupInfo.SourceIP)
							destinationHostName := resolveHostnameWithCache(data.LookupInfo.DestinationIP)

							fmt.Printf("SENDTO PID=%d NAME=%s %s/%s[%s]:%d -> %s[%s]:%d\n",
								data.SendMessage.ProcessID,
								data.SendMessage.CommandName,
								conversationProtocol,
								destinationHostName,
								data.LookupInfo.DestinationIP,
								data.LookupInfo.DestinationPort,
								sourceHostName,
								data.LookupInfo.SourceIP,
								data.LookupInfo.SourcePort,
							)
							fmt.Printf("SENDTO PID=%d NAME=%s %s/%s[%s]:%d <- %s[%s]:%d\n",
								data.ReceiveMessage.ProcessID,
								data.ReceiveMessage.CommandName,
								conversationProtocol,
								destinationHostName,
								data.LookupInfo.DestinationIP,
								data.LookupInfo.DestinationPort,
								sourceHostName,
								data.LookupInfo.SourceIP,
								data.LookupInfo.SourcePort,
							)
							// Освобождаем память и возвращаем EventData в пул
							delete(eventMapIPv4Reverse, port)
							eventDataPool.Put(data)
						}
					} else if family == 10 {
						// Обработка IPv6-событий (sys_exit_sendto)
						port := event.Dport
						processID := event.Pid
						ipv6Destination := pkg.IPv6FromLEWords(IPv6BytesToWords(event.DstIP6.In6U.U6Addr8))
						fmt.Printf("SENDTO IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
							processID,
							ipv6Destination.String(),
							port,
							pkg.Int8ToString(event.Comm),
						)
					}
				}

				// Sysexit == 11: sys_exit_sendmsg
				if event.Sysexit == 11 {
					if event.Family == 2 {
						port := int(event.Dport)
						data, exists := eventMapIPv4[port]
						if !exists {
							data = eventDataPool.Get().(*EventData)
							*data = EventData{}
							eventMapIPv4[port] = data
						}
						data.SendMessage = SendmsgEvent{
							DestinationIP:   destinationIPv4,
							DestinationPort: port,
							ProcessID:       event.Pid,
							CommandName:     pkg.Int8ToString(event.Comm),
							ProtocolNumber:  int(event.Proto),
						}
						data.HasSendMessage = true

						if data.HasLookup && data.HasReceive {
							conversationProtocol := determineProtocolName(data.LookupInfo.ProtocolNumber)
							sourceHostName := resolveHostnameWithCache(data.LookupInfo.SourceIP)
							destinationHostName := resolveHostnameWithCache(data.LookupInfo.DestinationIP)

							fmt.Println("")
							fmt.Printf("SENDMSG PID=%d NAME=%s %s/%s:%d->%s:%d\n",
								data.SendMessage.ProcessID,
								data.SendMessage.CommandName,
								conversationProtocol,
								data.LookupInfo.DestinationIP,
								data.LookupInfo.DestinationPort,
								data.LookupInfo.SourceIP,
								data.LookupInfo.SourcePort,
							)
							fmt.Printf("SENDMSG PID=%d NAME=%s %s/%s:%d<-%s:%d\n",
								data.ReceiveMessage.ProcessID,
								data.ReceiveMessage.CommandName,
								conversationProtocol,
								data.LookupInfo.DestinationIP,
								data.LookupInfo.DestinationPort,
								data.LookupInfo.SourceIP,
								data.LookupInfo.SourcePort,
							)
							fmt.Println("")
							delete(eventMapIPv4, port)
							eventDataPool.Put(data)
						}
					} else if event.Family == 10 {
						port := event.Dport
						processID := event.Pid
						ipv6Destination := pkg.IPv6FromLEWords(IPv6BytesToWords(event.DstIP6.In6U.U6Addr8))
						fmt.Printf("SENDMSG IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
							processID,
							ipv6Destination.String(),
							port,
							pkg.Int8ToString(event.Comm),
						)
					}
				}

				// Sysexit == 2: sys_exit_recvfrom
				if event.Sysexit == 2 {
					if event.Family == 2 {
						port := int(event.Sport)
						data, exists := eventMapIPv4[port]
						if !exists {
							data = eventDataPool.Get().(*EventData)
							*data = EventData{}
							eventMapIPv4[port] = data
						}
						data.ReceiveMessage = RecvmsgEvent{
							SourceIP:       sourceIPv4,
							SourcePort:     port,
							ProcessID:      event.Pid,
							CommandName:    pkg.Int8ToString(event.Comm),
							ProtocolNumber: int(event.Proto),
						}
						data.HasReceive = true

						if data.HasLookup && data.HasSendMessage {
							conversationProtocol := determineProtocolName(data.LookupInfo.ProtocolNumber)
							sourceHostName := resolveHostnameWithCache(data.LookupInfo.SourceIP)
							destinationHostName := resolveHostnameWithCache(data.LookupInfo.DestinationIP)

							fmt.Printf("RECVFROM PID=%d NAME=%s %s/%s[%s]:%d -> %s[%s]:%d\n",
								data.SendMessage.ProcessID,
								data.SendMessage.CommandName,
								conversationProtocol,
								destinationHostName,
								data.LookupInfo.DestinationIP,
								data.LookupInfo.DestinationPort,
								sourceHostName,
								data.LookupInfo.SourceIP,
								data.LookupInfo.SourcePort,
							)
							fmt.Printf("RECVFROM PID=%d NAME=%s %s/%s[%s]:%d <- %s[%s]:%d\n",
								data.ReceiveMessage.ProcessID,
								data.ReceiveMessage.CommandName,
								conversationProtocol,
								destinationHostName,
								data.LookupInfo.DestinationIP,
								data.LookupInfo.DestinationPort,
								sourceHostName,
								data.LookupInfo.SourceIP,
								data.LookupInfo.SourcePort,
							)
							delete(eventMapIPv4, port)
							eventDataPool.Put(data)
						}
					} else if event.Family == 10 {
						port := event.Sport
						processID := event.Pid
						ipv6Source := pkg.IPv6FromLEWords(IPv6BytesToWords(event.SrcIP6.In6U.U6Addr8))
						fmt.Printf("RECVFROM IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
							processID,
							ipv6Source.String(),
							port,
							pkg.Int8ToString(event.Comm),
						)
					}
				}

				// Sysexit == 12: sys_exit_recvmsg
				if event.Sysexit == 12 {
					if event.Family == 2 {
						port := int(event.Sport)
						data, exists := eventMapIPv4[port]
						if !exists {
							data = eventDataPool.Get().(*EventData)
							*data = EventData{}
							eventMapIPv4[port] = data
						}
						data.ReceiveMessage = RecvmsgEvent{
							SourceIP:       sourceIPv4,
							SourcePort:     port,
							ProcessID:      event.Pid,
							CommandName:    pkg.Int8ToString(event.Comm),
							ProtocolNumber: int(event.Proto),
						}
						data.HasReceive = true

						if data.HasLookup && data.HasSendMessage {
							conversationProtocol := determineProtocolName(data.LookupInfo.ProtocolNumber)
							sourceHostName := resolveHostnameWithCache(data.LookupInfo.SourceIP)
							destinationHostName := resolveHostnameWithCache(data.LookupInfo.DestinationIP)

							fmt.Println("")
							fmt.Printf("%s/%s:%d->%s:%d\n",
								conversationProtocol,
								data.LookupInfo.DestinationIP,
								data.LookupInfo.DestinationPort,
								data.LookupInfo.SourceIP,
								data.LookupInfo.SourcePort,
							)
							fmt.Printf("%s/%s:%d<-%s:%d\n",
								conversationProtocol,
								data.LookupInfo.DestinationIP,
								data.LookupInfo.DestinationPort,
								data.LookupInfo.SourceIP,
								data.LookupInfo.SourcePort,
							)
							fmt.Println("")
							delete(eventMapIPv4, port)
							eventDataPool.Put(data)
						}
					} else if event.Family == 10 {
						port := event.Sport
						processID := event.Pid
						ipv6Source := pkg.IPv6FromLEWords(IPv6BytesToWords(event.SrcIP6.In6U.U6Addr8))
						fmt.Printf("RECVMSG IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
							processID,
							ipv6Source.String(),
							port,
							pkg.Int8ToString(event.Comm),
						)
					}
				}

				// Sysexit == 3: sk_lookup
				if event.Sysexit == 3 {
					family := event.Family
					if family == 2 {
						port := int(event.Dport)
						data, exists := eventMapIPv4[port]
						if !exists {
							data = eventDataPool.Get().(*EventData)
							*data = EventData{}
							eventMapIPv4[port] = data
						}
						data.LookupInfo = LookupEvent{
							SourceIP:        sourceIPv4,
							SourcePort:      int(event.Sport),
							DestinationIP:   destinationIPv4,
							DestinationPort: int(event.Dport),
							ProtocolNumber:  int(event.Proto),
						}
						data.HasLookup = true

						// Для sendto-обработки нам нужен кэш и для «оборотного» мэппинга
						reversePort := int(event.Sport)
						dataReverse, existsReverse := eventMapIPv4Reverse[reversePort]
						if !existsReverse {
							dataReverse = eventDataPool.Get().(*EventData)
							*dataReverse = EventData{}
							eventMapIPv4Reverse[reversePort] = dataReverse
						}
						dataReverse.LookupInfo = data.LookupInfo
						dataReverse.HasLookup = true

						if data.HasReceive && data.HasSendMessage {
							conversationProtocol := determineProtocolName(data.LookupInfo.ProtocolNumber)
							sourceHostName := resolveHostnameWithCache(data.LookupInfo.SourceIP)
							destinationHostName := resolveHostnameWithCache(data.LookupInfo.DestinationIP)

							fmt.Println("")
							fmt.Printf("LOOKUP PID=%d NAME=%s %s/%s[%s]:%d<-%s[%s]:%d\n",
								data.SendMessage.ProcessID,
								data.SendMessage.CommandName,
								conversationProtocol,
								destinationHostName,
								data.LookupInfo.DestinationIP,
								data.LookupInfo.DestinationPort,
								sourceHostName,
								data.LookupInfo.SourceIP,
								data.LookupInfo.SourcePort,
							)
							fmt.Printf("LOOKUP PID=%d NAME=%s %s/%s[%s]:%d->%s[%s]:%d\n",
								data.ReceiveMessage.ProcessID,
								data.ReceiveMessage.CommandName,
								conversationProtocol,
								destinationHostName,
								data.LookupInfo.DestinationIP,
								data.LookupInfo.DestinationPort,
								sourceHostName,
								data.LookupInfo.SourceIP,
								data.LookupInfo.SourcePort,
							)
							fmt.Println("")
							delete(eventMapIPv4, port)
							eventDataPool.Put(data)
						}
					} else if family == 10 {
						ipv6Source := pkg.IPv6FromLEWords(IPv6BytesToWords(event.SrcIP6.In6U.U6Addr8))
						fmt.Printf("LOOKUP SRC IPv6=%s\n", ipv6Source)
						ipv6Destination := pkg.IPv6FromLEWords(IPv6BytesToWords(event.DstIP6.In6U.U6Addr8))
						fmt.Printf("LOOKUP DST IPv6=%s\n", ipv6Destination)
						fmt.Printf("LOOKUP SPORT=%d  DPORT=%d PROTO=%d\n", event.Sport, event.Dport, event.Proto)
					}
				}

				// Sysexit == 6: inet_sock_set_state
				if event.Sysexit == 6 {
					// Обработка IPv6 TCP-событий
					if event.Family == 10 {
						sourcePort := event.Sport
						destinationPort := event.Dport
						processID := event.Pid
						ipv6Source := pkg.IPv6FromLEWords(IPv6BytesToWords(event.SrcIP6.In6U.U6Addr8))
						fmt.Printf("TCP IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
							processID,
							ipv6Source.String(),
							sourcePort,
							pkg.Int8ToString(event.Comm),
						)
						ipv6Destination := pkg.IPv6FromLEWords(IPv6BytesToWords(event.DstIP6.In6U.U6Addr8))
						fmt.Printf("TCP IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
							processID,
							ipv6Destination,
							destinationPort,
							pkg.Int8ToString(event.Comm),
						)
						fmt.Printf("!!!!!TCP IPv6 !!!!!!PID=%d  NAME=%s\n",
							event.Pid,
							pkg.Int8ToString(event.Comm),
						)
					}

					// Обработка состояний TCP (ESTABLISHED, SYN_SENT и т. д.)
					if event.State == 1 {
						channelMutex.Lock()
						select {
						case portStateChannel <- int(event.Sport):
						default:
							portStateChannel <- int(event.Sport)
							fmt.Printf("State 1: порт %d был заменен\n", event.Sport)
						}
						channelMutex.Unlock()

						var resolvedDestinationHost string
						if event.DstIP.IsLoopback() {
							resolvedDestinationHost = "localhost"
						} else {
							dnsName, err := resolveHostnameWithError(event.DstIP)
							if err != nil {
								resolvedDestinationHost = "unknown"
							} else {
								resolvedDestinationHost = dnsName
							}
						}

						resolvedSourceHost := resolveHostnameWithCache(event.SrcIP)
						sourceAddress := fmt.Sprintf("//%s[%s]:%d", resolvedSourceHost, event.SrcIP.String(), event.Sport)
						destinationAddress := fmt.Sprintf("//%s[%s]:%d", resolvedDestinationHost, event.DstIP.String(), event.Dport)

						if event.Proto == 6 {
							protocolString = "TCP"
						}

						fmt.Println("")
						fmt.Printf("PID=%d %s:%s <- %s:%s \n",
							event.Pid,
							protocolString,
							sourceAddress,
							protocolString,
							destinationAddress,
						)
					}

					// Сигналы о закрытии TCP (FIN, RST)
					if event.State == 2 || event.State == 10 {
						channelMutex.Lock()
						select {
						case processIDChannel <- int(event.Pid):
						default:
						}
						channelMutex.Unlock()
					}

					// Пробуем забрать порт из канала portStateChannel
					select {
					case portValue := <-portStateChannel:
						var resolvedDestinationHost string
						if event.DstIP.IsLoopback() {
							resolvedDestinationHost = resolveHostnameWithCache(event.DstIP)
						} else {
							dnsName, err := resolveHostnameWithError(event.DstIP)
							if err != nil {
								resolvedDestinationHost = "unknown"
							} else {
								resolvedDestinationHost = dnsName
							}
						}

						resolvedSourceHost := resolveHostnameWithCache(event.SrcIP)
						sourceAddress := fmt.Sprintf("//%s[%s]:%d", resolvedSourceHost, event.SrcIP.String(), portValue)
						destinationAddress := fmt.Sprintf("//%s[%s]:%d", resolvedDestinationHost, event.DstIP.String(), event.Dport)

						var processIDValue int
						select {
						case processIDValue = <-processIDChannel:
						default:
						}

						if event.Proto == 6 {
							protocolString = "TCP"
						}

						fmt.Printf("PID=%d %s:%s -> %s:%s \n",
							processIDValue,
							protocolString,
							sourceAddress,
							protocolString,
							destinationAddress,
						)
						fmt.Println("")

					default:
						fmt.Println("")
					}
				}

			} // конец обработки batchRecords
		} // конец бесконечного цикла
	}()

	fmt.Println("Нажмите Ctrl+C для завершения")
	<-stopSignalChannel
	fmt.Println("Завершение работы...")
}

// IPv6BytesToWords преобразует массив из 16 байтов IPv6 в массив из 4 uint32
func IPv6BytesToWords(addr [16]uint8) [4]uint32 {
	var words [4]uint32
	for i := 0; i < 4; i++ {
		words[i] = uint32(addr[i*4]) |
			uint32(addr[i*4+1])<<8 |
			uint32(addr[i*4+2])<<16 |
			uint32(addr[i*4+3])<<24
	}
	return words
}

// determineProtocolName возвращает читаемую строку для числового кода протокола
func determineProtocolName(protocolNumber int) string {
	switch protocolNumber {
	case 6:
		return "TCP"
	case 17:
		return "UDP"
	default:
		return fmt.Sprintf("PROTO%d", protocolNumber)
	}
}

// resolveHostnameWithCache возвращает DNS-имя для IP из кэша или делает реальный DNS-запрос
func resolveHostnameWithCache(ip net.IP) string {
	ipString := ip.String()

	dnsCache.RLock()
	if entry, exists := dnsCache.entries[ipString]; exists {
		if time.Since(entry.lastUpdated) < dnsCacheTTL {
			dnsCache.RUnlock()
			return entry.hostName
		}
	}
	dnsCache.RUnlock()

	// Если в кэше нет записи или она устарела — обновляем
	dnsName, err := resolveHostnameWithError(ip)
	if err != nil {
		return "unknown"
	}

	dnsCache.Lock()
	dnsCache.entries[ipString] = cachedDNSRecord{
		hostName:    dnsName,
		lastUpdated: time.Now(),
	}
	dnsCache.Unlock()

	return dnsName
}

// resolveHostnameWithError делает реальный DNS-запрос и возвращает ошибку, если она произошла
func resolveHostnameWithError(ip net.IP) (string, error) {
	names, err := net.LookupAddr(ip.String())
	if err != nil || len(names) == 0 {
		return "", err
	}
	// net.LookupAddr возвращает список возможных имен, берем первое
	return names[0], nil
}











