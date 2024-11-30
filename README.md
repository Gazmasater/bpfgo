tracepoint:net:udp_rcv
{
    $src_ip = args->saddr;
    $dst_ip = args->daddr;
    
    printf("UDP packet received from IP: %d.%d.%d.%d:%d to IP: %d.%d.%d.%d:%d\n",
           ($src_ip >> 24) & 0xFF, ($src_ip >> 16) & 0xFF, ($src_ip >> 8) & 0xFF, $src_ip & 0xFF, args->sport,
           ($dst_ip >> 24) & 0xFF, ($dst_ip >> 16) & 0xFF, ($dst_ip >> 8) & 0xFF, $dst_ip & 0xFF, args->dport);
}
