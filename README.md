
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



Time: 2025-06-01 18:44:04 MSK
Duration: 30.01s, Total samples = 80ms ( 0.27%)
Entering interactive mode (type "help" for commands, "o" for options)
(pprof) top
Showing nodes accounting for 80ms, 100% of 80ms total
Showing top 10 nodes out of 79
      flat  flat%   sum%        cum   cum%
      20ms 25.00% 25.00%       20ms 25.00%  internal/runtime/syscall.Syscall6
      20ms 25.00% 50.00%       20ms 25.00%  runtime.cgocall
      10ms 12.50% 62.50%       10ms 12.50%  fmt.(*buffer).write (inline)
      10ms 12.50% 75.00%       10ms 12.50%  runtime.futex
      10ms 12.50% 87.50%       10ms 12.50%  runtime.memmove
      10ms 12.50%   100%       10ms 12.50%  runtime.pcdatavalue1
         0     0%   100%       20ms 25.00%  bpfgo/pkg.ResolveIP
         0     0%   100%       40ms 50.00%  bpfgo/pkg.ResolveIP_n
         0     0%   100%       10ms 12.50%  fmt.(*buffer).writeString
         0     0%   100%       10ms 12.50%  fmt.(*fmt).fmtInteger
(pprof) 


batch := make([]perf.Record, 0, batchSize)
for {
    batch = batch[:0] // обнуляем длину, оставляем capacity = batchSize

    for i := 0; i < batchSize; i++ {
        record, err := rd.Read()
        if err != nil {
            if errors.Is(err, os.ErrDeadlineExceeded) {
                break
            }
            log.Fatalf("error reading from perf reader: %v", err)
        }
        batch = append(batch, record)
        if len(batch) >= batchSize {
            break
        }
    }
    if len(batch) == 0 {
        continue
    }

    for _, record := range batch {
        …
    }
}


az358@gaz358-BOD-WXX9:~/myprog/bpfgo$ go tool pprof http://localhost:6060/debug/pprof/heap
Fetching profile over HTTP from http://localhost:6060/debug/pprof/heap
Saved profile in /home/gaz358/pprof/pprof.bpfgo.alloc_objects.alloc_space.inuse_objects.inuse_space.004.pb.gz
File: bpfgo
Build ID: 6a9bbe59327e8bfde36cf53a4755eb7f471aefae
Type: inuse_space
Time: 2025-06-01 22:32:09 MSK
Entering interactive mode (type "help" for commands, "o" for options)
(pprof) top
Showing nodes accounting for 22090.34kB, 100% of 22090.34kB total
Showing top 10 nodes out of 30
      flat  flat%   sum%        cum   cum%
 6945.99kB 31.44% 31.44% 12073.43kB 54.65%  github.com/cilium/ebpf/btf.readAndInflateTypes
 5127.44kB 23.21% 54.65%  5127.44kB 23.21%  github.com/cilium/ebpf/btf.readAndInflateTypes.func2
 4096.09kB 18.54% 73.20%  4096.09kB 18.54%  bufio.(*Scanner).Text (inline)
 3356.76kB 15.20% 88.39%  7452.85kB 33.74%  github.com/cilium/ebpf/btf.readStringTable
    2052kB  9.29% 97.68%     2052kB  9.29%  runtime.allocm
  512.05kB  2.32%   100% 20038.34kB 90.71%  runtime.main
         0     0%   100% 19526.28kB 88.39%  github.com/cilium/ebpf.(*CollectionSpec).LoadAndAssign
         0     0%   100% 19526.28kB 88.39%  github.com/cilium/ebpf.(*CollectionSpec).LoadAndAssign.func1
         0     0%   100% 19526.28kB 88.39%  github.com/cilium/ebpf.(*collectionLoader).loadProgram
         0     0%   100% 19526.28kB 88.39%  github.com/cilium/ebpf.applyRelocations
(pprof) 

