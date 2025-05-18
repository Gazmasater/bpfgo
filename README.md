__u32 src = 0;
BPF_CORE_READ(src, ctx, local_ip4);
info.srcIP.s_addr = src;





