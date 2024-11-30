Использование bpftrace
Убедитесь, что у вас установлен bpftrace. На большинстве систем Linux его можно установить через пакетный менеджер. Например, для Ubuntu:

bash
Копировать код
sudo apt install bpfcc-tools bpftrace
Напишите или выполните простой скрипт bpftrace для отслеживания UDP пакетов. Сохраните его в файл, например, trace_udp.bpf:

bpftrace
Копировать код
tracepoint:net:udp_rcv
{
    printf("UDP packet received from IP: %s:%d to IP: %s:%d\n",
           inet_ntoa(args->saddr), args->sport, inet_ntoa(args->daddr), args->dport);
}
Запустите скрипт через bpftrace:

bash
Копировать код
sudo bpftrace trace_udp.bpf
Это позволит вам отслеживать все UDP пакеты, которые проходят через вашу систему, и выводить информацию о полученных пакетах (включая IP-адреса и порты).

az358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo bpftrace trace_udp.bpf
trace_udp.bpf:4:12-21: ERROR: Unknown function: inet_ntoa
           inet_ntoa(args->saddr), args->sport, inet_ntoa(args->daddr), args->dport);
           ~~~~~~~~~
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 
