
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
go tool pprof http://localhost:6060/debug/pprof/heap



"net/http"      // добавлено для pprof
	_ "net/http/pprof" // регистрирует pprof-эндпоинты

     _ "net/http/pprof" // blank-import: регистрирует pprof-хэндлеры в DefaultServeMux



git rm -r --cached TMP
git commit -m "Ignore папку TMP"

git rm -r --cached .vscode
git commit -m "Игнорировать папку .vscode"

Time: 2025-06-02 01:19:22 MSK
Duration: 30.02s, Total samples = 30ms (  0.1%)
Entering interactive mode (type "help" for commands, "o" for options)
(pprof) top
Showing nodes accounting for 30ms, 100% of 30ms total
Showing top 10 nodes out of 26
      flat  flat%   sum%        cum   cum%
      10ms 33.33% 33.33%       10ms 33.33%  github.com/cilium/ebpf/perf.(*forwardReader).Read
      10ms 33.33% 66.67%       10ms 33.33%  internal/runtime/syscall.Syscall6
      10ms 33.33%   100%       10ms 33.33%  runtime.cgocall
         0     0%   100%       10ms 33.33%  bpfgo/pkg.ResolveIP
         0     0%   100%       10ms 33.33%  github.com/cilium/ebpf/internal/epoll.(*Poller).Wait
         0     0%   100%       10ms 33.33%  github.com/cilium/ebpf/internal/unix.EpollWait (inline)
         0     0%   100%       20ms 66.67%  github.com/cilium/ebpf/perf.(*Reader).Read
         0     0%   100%       20ms 66.67%  github.com/cilium/ebpf/perf.(*Reader).ReadInto
         0     0%   100%       10ms 33.33%  github.com/cilium/ebpf/perf.(*Reader).readRecordFromRing
         0     0%   100%       10ms 33.33%  github.com/cilium/ebpf/perf.readRecord
(pprof) 





