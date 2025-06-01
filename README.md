
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



	go func() {
		log.Println("Starting pprof HTTP server on :6060")
		if err := http.ListenAndServe(":6060", nil); err != nil {
			log.Fatalf("pprof ListenAndServe error: %v", err)
		}
	}()

 go func() {
    log.Println("Starting pprof HTTP server on :6060")
    if err := http.ListenAndServe(":6060", nil); err != nil {
        log.Fatalf("pprof ListenAndServe error: %v", err)
    }
}()



curl http://localhost:6060/debug/pprof/

File: bpfgo
Build ID: be22e3472fc86bd633e4aa81a27b3e4da8c8011a
Type: cpu
Time: 2025-06-01 15:42:44 MSK
Duration: 30.01s, Total samples = 70ms ( 0.23%)
Entering interactive mode (type "help" for commands, "o" for options)
(pprof) top
Showing nodes accounting for 70ms, 100% of 70ms total
Showing top 10 nodes out of 54
      flat  flat%   sum%        cum   cum%
      20ms 28.57% 28.57%       20ms 28.57%  internal/runtime/syscall.Syscall6
      10ms 14.29% 42.86%       40ms 57.14%  github.com/miekg/dns.(*Client).Exchange
      10ms 14.29% 57.14%       10ms 14.29%  internal/poll.(*FD).Write
      10ms 14.29% 71.43%       10ms 14.29%  runtime.(*timer).stop
      10ms 14.29% 85.71%       10ms 14.29%  runtime.futex
      10ms 14.29%   100%       10ms 14.29%  runtime.memclrNoHeapPointers
         0     0%   100%       10ms 14.29%  bpfgo/pkg.ResolveIP
         0     0%   100%       40ms 57.14%  bpfgo/pkg.ResolveIP_n
         0     0%   100%       10ms 14.29%  fmt.Fprintf
         0     0%   100%       10ms 14.29%  fmt.Printf
(pprof) 










