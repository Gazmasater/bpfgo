if (BPF_CORE_READ_INTO(info.comm, conn_info, comm) < 0)
    goto cleanup;


__s64 ret;
if (BPF_CORE_READ_INTO(&ret, ctx, ret) < 0)
    return 0;
if (ret < 0)
    goto cleanup;
    

