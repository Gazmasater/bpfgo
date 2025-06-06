https://github.com/Morwran/nft-go/tree/main/internal/expr-encoders



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


func resolveHost(ip net.IP) string {

	key := ip.String()

	cacheMu.RLock()
	if host, ok := resolveCache[key]; ok {
		cacheMu.RUnlock()
		return host
	}
	cacheMu.RUnlock()

	var host string
	if ip.To4() != nil {
		if ip.IsLoopback() {
			host = "localhost"
		} else {
			host = pkg.ResolveIP(ip)

		}
	} else {
		var err error
		host, err = pkg.ResolveIP_n(ip)
		if err != nil {
			host = "unknown"
		}
	}

	cacheMu.Lock()
	resolveCache[key] = host
	cacheMu.Unlock()

	return host
}







_______________________________________________________________________________________________


go tool pprof http://localhost:6060/debug/pprof/profile?seconds=30
go tool pprof http://localhost:6060/debug/pprof/heap



"net/http"      // добавлено для pprof
	_ "net/http/pprof" // регистрирует pprof-эндпоинты

     _ "net/http/pprof" // blank-import: регистрирует pprof-хэндлеры в DefaultServeMux



git checkout ProcNet_monitor
git push --force origin ProcNet_monitor


______________________________________________________________________________________________

sudo apt install mitmproxy

mitmproxy
http://mitm.it




✅ Цель:
Зайти на https://ozon.ru в браузере.

Перехватить и увидеть все отправленные запросы и то, что отправилось сайту: заголовки, cookies, IP и прочее.

🧰 Что нужно:
mitmproxy (или mitmweb)

Сертификат установлен в систему или браузер

Браузер настроен на использование прокси

🔧 Шаг 1: Запусти mitmproxy (или mitmweb)
В терминале:

mitmweb
Это откроет веб-интерфейс: http://127.0.0.1:8081

Прокси работает на 127.0.0.1:8080

🔧 Шаг 2: Настрой браузер на использование прокси
В Firefox:
Настройки → Сеть → Настроить соединение

Прокси-сервер:

HTTP Proxy: 127.0.0.1

Port: 8080

Также использовать для HTTPS

В Chrome (через системные настройки):
Linux/macOS:

google-chrome --proxy-server="http=127.0.0.1:8080;https=127.0.0.1:8080"
🔧 Шаг 3: Установи сертификат mitmproxy, если не установил
Перейди в браузере на:

http://mitm.it
Выбери Linux/macOS → скачай и установи сертификат (инструкции выше).

🔎 Шаг 4: Зайди вручную на https://www.ozon.ru
Открой в браузере Ozon, полазай по сайту (главная, авторизация, каталог и т.д.)

Все запросы будут автоматически видны в mitmproxy / mitmweb.

👁 Что смотреть в mitmweb
Перейди на http://127.0.0.1:8081:

Нажми на нужный запрос → справа:

Request → заголовки (User-Agent, Cookie, и т.д.)

Response → тело ответа

Также можно экспортировать запрос или сохранить лог.

🧠 Что это даёт
Теперь ты точно видишь, что получает сайт:

Твои cookies и ID сессии

Что отправляется при входе, поиске, фильтрации и т.д.

Можно сохранить User-Agent, Cookie и использовать их в автоматических скриптах позже
________________________________________________________________________________




gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ go test 
--- FAIL: Test_CmpEncoderAdvanced (0.00s)
    --- FAIL: Test_CmpEncoderAdvanced/Test_CmpEncodeIR (0.00s)
        --- FAIL: Test_CmpEncoderAdvanced/Test_CmpEncodeIR/payload_ip_version_!=_5 (0.00s)
            panic.go:262: test panicked: runtime error: invalid memory address or nil pointer dereference
                goroutine 15 [running]:
                runtime/debug.Stack()
                        /home/gaz358/go/pkg/mod/golang.org/toolchain@v0.0.1-go1.24.2.linux-amd64/src/runtime/debug/stack.go:26 +0x5e
                github.com/stretchr/testify/suite.failOnPanic(0xc0002728c0, {0x8b5b00, 0xd2f540})
                        /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:89 +0x37
                github.com/stretchr/testify/suite.recoverAndFailOnPanic(0xc0002728c0)
                        /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:83 +0x35
                panic({0x8b5b00?, 0xd2f540?})
                        /home/gaz358/go/pkg/mod/golang.org/toolchain@v0.0.1-go1.24.2.linux-amd64/src/runtime/panic.go:792 +0x132
                github.com/Morwran/nft-go/internal/expr-encoders.includeHeaderIfKnown(...)
                        /home/gaz358/myprog/nft-go/internal/expr-encoders/payload.go:213
                github.com/Morwran/nft-go/internal/expr-encoders.(*payloadEncoder).buildLRFromCmpData(0x7d9d7ae592b8?, 0xc0001ff520, 0xc0001ff540)
                        /home/gaz358/myprog/nft-go/internal/expr-encoders/payload.go:158 +0x30
                github.com/Morwran/nft-go/internal/expr-encoders.(*cmpEncoder).formatCmpLR(0x7d9d7afc0108?, 0x20?, {{0x93bfc7, 0xa}, 0x0, {0x9d7a80, 0xc00002c900}, {0x0, 0x0}, {0x0, ...}})
                        /home/gaz358/myprog/nft-go/internal/expr-encoders/cmp.go:109 +0x134
                github.com/Morwran/nft-go/internal/expr-encoders.(*cmpEncoder).EncodeIR(0xc000070ea8, 0xc0001ff520)
                        /home/gaz358/myprog/nft-go/internal/expr-encoders/cmp.go:36 +0x174
                github.com/Morwran/nft-go/internal/expr-encoders.(*cmpEncoderAdvancedTestSuite).Test_CmpEncodeIR.func4()
                        /home/gaz358/myprog/nft-go/internal/expr-encoders/encodersCmp_test.go:80 +0x67
                github.com/stretchr/testify/suite.(*Suite).Run.func1(0xc0002728c0)
                        /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:115 +0x169
                testing.tRunner(0xc0002728c0, 0xc0001ff500)
                        /home/gaz358/go/pkg/mod/golang.org/toolchain@v0.0.1-go1.24.2.linux-amd64/src/testing/testing.go:1792 +0xf4
                created by testing.(*T).Run in goroutine 13
                        /home/gaz358/go/pkg/mod/golang.org/toolchain@v0.0.1-go1.24.2.linux-amd64/src/testing/testing.go:1851 +0x413
--- FAIL: Test_CtEncoderAdvanced (0.00s)
    --- FAIL: Test_CtEncoderAdvanced/Test_CtEncodeIR_Complex (0.00s)
        --- FAIL: Test_CtEncoderAdvanced/Test_CtEncodeIR_Complex/ct_mark_set_42 (0.00s)
            encodersCt_test.go:115: 
                        Error Trace:    /home/gaz358/myprog/nft-go/internal/expr-encoders/encodersCt_test.go:115
                                                                /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:115
                        Error:          Not equal: 
                                        expected: "ct mark set 42"
                                        actual  : "ct mark set *"
                                    
                                        Diff:
                                        --- Expected
                                        +++ Actual
                                        @@ -1 +1 @@
                                        -ct mark set 42
                                        +ct mark set *
                        Test:           Test_CtEncoderAdvanced/Test_CtEncodeIR_Complex/ct_mark_set_42
        --- FAIL: Test_CtEncoderAdvanced/Test_CtEncodeIR_Complex/ct_status_snat,dnat,confirmed (0.00s)
            encodersCt_test.go:115: 
                        Error Trace:    /home/gaz358/myprog/nft-go/internal/expr-encoders/encodersCt_test.go:115
                                                                /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:115
                        Error:          Not equal: 
                                        expected: "ct status confirmed,dnat,snat"
                                        actual  : "ct status assured,confirmed,snat,dnat"
                                    
                                        Diff:
                                        --- Expected
                                        +++ Actual
                                        @@ -1 +1 @@
                                        -ct status confirmed,dnat,snat
                                        +ct status assured,confirmed,snat,dnat
                        Test:           Test_CtEncoderAdvanced/Test_CtEncodeIR_Complex/ct_status_snat,dnat,confirmed
--- FAIL: Test_ExthdrEncoder (0.00s)
    --- FAIL: Test_ExthdrEncoder/Test_ExthdrEncodeIR_ValidOnly (0.00s)
        --- FAIL: Test_ExthdrEncoder/Test_ExthdrEncodeIR_ValidOnly/exthdr_read_and_compare_(source_+_rhs) (0.00s)
            encodersExthdr_test.go:71: 
                        Error Trace:    /home/gaz358/myprog/nft-go/internal/expr-encoders/encodersExthdr_test.go:71
                                                                /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:115
                        Error:          Not equal: 
                                        expected: "exthdr @4,8,2 set 0x1234"
                                        actual  : "ip option @4,8,2 set 0x1234"
                                    
                                        Diff:
                                        --- Expected
                                        +++ Actual
                                        @@ -1 +1 @@
                                        -exthdr @4,8,2 set 0x1234
                                        +ip option @4,8,2 set 0x1234
                        Test:           Test_ExthdrEncoder/Test_ExthdrEncodeIR_ValidOnly/exthdr_read_and_compare_(source_+_rhs)
[{"match":{"op":"==","left":{"meta":{"key":"l4proto"}},"right":"tcp"}},{"counter":{"bytes":0,"packets":0}},{"log":null},{"accept":null}]
[{"match":{"op":"!=","left":{"meta":{"key":"oifname"}},"right":"lo"}},{"mangle":{"key":{"meta":{"key":"nftrace"}},"value":1}},{"goto":{"target":"FW-OUT"}}]
meta l4proto tcp counter packets 0 bytes 0 log accept
ip version != 5
ip daddr @ipSet
ip daddr != 93.184.216.34 meta l4proto tcp dport {80,443} meta l4proto tcp
th dport != 80
meta l4proto tcp dport != 80
meta l4proto tcp sport >= 80 sport <= 100
meta nftrace set 1 ip daddr 10.0.0.0/8 meta l4proto udp
meta l4proto icmp type echo-reply
ct state established,related
ct expiration 1s
ct direction original
ct l3proto ipv4
ct protocol tcp
FAIL
exit status 1
FAIL    github.com/Morwran/nft-go/internal/expr-encoders        0.019s





