

__s64 ret;
if (BPF_CORE_READ_INTO(&ret, ctx, ret) < 0)
    return 0;
if (ret < 0)
    goto cleanup;


            __builtin_memcpy(&info.srcIP6, &sa6.sin6_addr.in6_u.u6_addr32, sizeof(info.srcIP6));
                __builtin_memcpy(&info.dstIP6, &sa6.sin6_addr.in6_u.u6_addr32, sizeof(info.dstIP6));


    

