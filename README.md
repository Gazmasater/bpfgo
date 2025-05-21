#pragma unroll
for (int i = 0; i < 4; i++) {
    info.srcIP6.in6_u.u6_addr32[i] = ctx->local_ip6[i];
    info.dstIP6.in6_u.u6_addr32[i] = ctx->remote_ip6[i];
}
