
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



Похоже, у тебя несколько перекрёстных проблем — давай по порядку:

Опечатка в имени поля
В коде ты пишешь


data.HasRecvmdg = true
а в структуре, скорее всего, поле называется HasRecvmsg (с «s»). Go-то тут ругнётся на “undefined: EventData.HasRecvmdg”. Проверь точное имя поля и поправь везде на одно и то же.

Неправильная map при заполнении eventMap_1
В первой ветке ты делаешь


data, ok := eventMap_1[port]
if !ok {
    data = EventData{}
    eventMap[port] = data    // <-- здесь должно быть eventMap_1!
}
Из-за этого часть данных идёт в eventMap, а не в eventMap_1, и потом при lookup ты не найдёшь нужную запись. Исправь на


eventMap_1[port] = data
Отсутствие пере­записи value-типа в map
Поскольку ты хранишь EventData не через указатели, а «по значению», при любом


data := eventMap[port]
data.HasLookup = true
ты меняешь только копию data, а в самой map ничего не меняется. Нужно после изменения снова сохранить:


data := eventMap[port]
data.HasRecvmsg = true
eventMap[port] = data
Необъявленные переменные proto, dsthost, srchost
В теле функций ты пишешь


proto = "UDP"
dsthost = pkg.ResolveIP(…)
но нигде не сделал


var proto, dsthost, srchost string
или не объявил их до использования. Добавь перед if event.Sysexit == ...:


var (
    proto   string
    dsthost string
    srchost string
)
После этих правок код будет компилироваться, а твои флаги HasLookup, HasSendmsg и HasRecvmsg начнут сохраняться корректно и по обеим картам (eventMap и eventMap_1).














