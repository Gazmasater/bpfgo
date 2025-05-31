
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



Итоговые правки
В ветке sys_exit=3 (lookup):


port := int(event.Dport)
data, exists := eventMap[port]
if !exists {
    data = &EventData{}
    eventMap[port] = data
}
data.Lookup = …
data.HasLookup = true

port_1 := int(event.Sport)
data_1, exists := eventMap_1[port_1]
if !exists {
    data_1 = &EventData{}

 eventMap_1[port_1] = data

 eventMap_1[port_1] = data_1
}
data_1.Lookup = …
data_1.HasLookup = true

В ветках sys_exit=1 и sys_exit=2 (sendto/sendmsg и recvfrom/recvmsg) убедись, что:

Чтение/запись из/в нужную карту идёт последовательно:

В sys_exit=1 ты читаешь из eventMap_1 (по dstPort), значит запись при необходимости — eventMap_1[port] = data.

В sys_exit=2/12 ты читаешь из eventMap (по srcPort), значит запись при необходимости — eventMap[port] = data.

Заменить все srchost := pkg.ResolveIP(srcIP) и dsthost := pkg.ResolveIP(…) в тех местах, где ты хочешь сохранить результат в глобальные переменные, на srchost = pkg.ResolveIP(srcIP) (без двоеточия). Аналогично с dsthost.

При “финальном” выводе (когда оба HasSendmsg + HasRecvmsg + HasLookup истинны) — лучше удалить сразу из обеих карт:

delete(eventMap, port)
delete(eventMap_1, port_1)  // где port_1 соответствует ключу в eventMap_1
После этих доработок проблема с “неожиданными” или «зависшими» записями исчезнет, и вывод всегда будет соответствовать фактическому порядку событий.


















