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


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ git rebase -i db77c5b 
[detached HEAD b310a4e] tmp
 Date: Mon Feb 10 20:37:10 2025 +0300
 4 files changed, 52 insertions(+), 89 deletions(-)
Successfully rebased and updated refs/heads/trace_core1.
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ git rebase -i db77c5b
[detached HEAD c3d8ba4] tmp
 Date: Mon Feb 10 20:37:10 2025 +0300
 3 files changed, 53 insertions(+), 87 deletions(-)
Successfully rebased and updated refs/heads/trace_core1.












