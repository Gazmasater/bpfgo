#pragma unroll
for (int i = 0; i < 4; i++) {
    info.srcIP6.in6_u.u6_addr32[i] = ctx->local_ip6[i];
    info.dstIP6.in6_u.u6_addr32[i] = ctx->remote_ip6[i];
}

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo ./bpfgo
[sudo] password for gaz358: 
2025/05/21 18:50:52 failed to load bpf objects: field LookUp: program look_up: load program: permission denied: dereference of modified ctx ptr R6 off=44 disallowed (55 line(s) omitted)
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 
