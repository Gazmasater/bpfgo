cat /sys/kernel/debug/tracing/events/net/udp_recvmsg/format

tracepoint:net:udp_rcv
{
    printf("UDP packet received from IP: %s:%d to IP: %s:%d\n",
           inet_ntoa(args->saddr), args->sport, inet_ntoa(args->daddr), args->dport);
}
