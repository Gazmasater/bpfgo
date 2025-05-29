
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


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ git push -u origin trace_core1 --force
Enumerating objects: 62, done.
Counting objects: 100% (62/62), done.
Delta compression using up to 8 threads
Compressing objects: 100% (55/55), done.
Writing objects: 100% (58/58), 158.39 MiB | 7.88 MiB/s, done.
Total 58 (delta 9), reused 23 (delta 1), pack-reused 0
remote: Resolving deltas: 100% (9/9), done.
remote: warning: See https://gh.io/lfs for more information.
remote: warning: File bpf/ecc is 64.14 MB; this is larger than GitHub's recommended maximum file size of 50.00 MB
remote: warning: File ebpf-nftrace.zip is 78.38 MB; this is larger than GitHub's recommended maximum file size of 50.00 MB
remote: warning: GH001: Large files detected. You may want to try Git Large File Storage - https://git-lfs.github.com.
To https://github.com/Gazmasater/bpfgo
 + dd97587...e25850b trace_core1 -> trace_core1 (forced update)
branch 'trace_core1' set up to track 'origin/trace_core1'.
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 
















