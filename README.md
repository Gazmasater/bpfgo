gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo tracepoint:net:udp_rcv
{
    printf("UDP packet received from IP: %s:%d to IP: %s:%d\n",
           inet_ntoa(args->saddr), args->sport, inet_ntoa(args->daddr), args->dport);
}
[sudo] password for gaz358: 
sudo: tracepoint:net:udp_rcv: command not found
bash: syntax error near unexpected token `"UDP packet received from IP: %s:%d to IP: %s:%d\n",'
bash: syntax error near unexpected token `args-'
bash: syntax error near unexpected token `}'
