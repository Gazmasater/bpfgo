  #pragma unroll
for (int i = 0; i < 4; i++) {
    __builtin_memcpy(&info->srcIP6.u6_addr8[i * 4], &ctx->local_ip6[i], 4);
    __builtin_memcpy(&info->dstIP6.u6_addr8[i * 4], &ctx->remote_ip6[i], 4);
}
