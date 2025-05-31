
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


struct trace_info {
    struct in_addr  srcIP;     //  4 байта
    struct in_addr  dstIP;     //  4 байта

    // Чтобы следующий массив in6_addr начинался на 8-байтной границе,
    // нужно вставить 0–pad (точное число зависит от текущей структуры).
    // Допустим, тут уже 8 байт занято, и in6_addr (16 байт) естественно выровнен.
    struct in6_addr srcIP6;    // 16 байт (выровнено по 4/8 байт)
    struct in6_addr dstIP6;    // 16 байт

    __u32           pid;       //  4 байта
    __u32           proto;     //  4 байта

    __u16           sport;     //  2 байта
    __u16           dport;     //  2 байта

    __u16           family;    //  2 байта
    __u8            sysexit;   //  1 байт
    __u8            state;     //  1 байт

    // Чтобы следующий массив char[32] начинался на 4-байтной границе,
    // после offset=54 нужно вставить 2 байта паддинга до 56:
    __u8            __pad0;    //  1 байт паддинга
    __u8            __pad1;    //  1 байт паддинга

    char            comm[32];  // 32 байта
    // no extra padding в конце, если sizeof == 88
};











