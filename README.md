  info.srcIP.s_addr = BPF_CORE_READ(ctx, saddr);
            info.dstIP.s_addr = BPF_CORE_READ(ctx, daddr);

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ bpf2go -output-dir . -tags linux -type trace_info -go-package=main -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)
/home/gaz358/myprog/bpfgo/trace.c:585:31: error: incompatible pointer to integer conversion assigning to '__be32' (aka 'unsigned int') from '__u8 *' (aka 'unsigned char *') [-Wint-conversion]
  585 |             info.srcIP.s_addr = BPF_CORE_READ(ctx, saddr);
      |                               ^ ~~~~~~~~~~~~~~~~~~~~~~~~~
/home/gaz358/myprog/bpfgo/trace.c:586:31: error: incompatible pointer to integer conversion assigning to '__be32' (aka 'unsigned int') from '__u8 *' (aka 'unsigned char *') [-Wint-conversion]
  586 |             info.dstIP.s_addr = BPF_CORE_READ(ctx, daddr);
      |                               ^ ~~~~~~~~~~~~~~~~~~~~~~~~~
2 errors generated.
Error: compile: exit status 1
gaz358@gaz358-BOD-WXX9:~/myprog/bpfg


