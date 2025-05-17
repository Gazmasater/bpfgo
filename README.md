        info.srcIP6[0]=(ctx->local_ip6[0]);
        info.srcIP6[1]=(ctx->local_ip6[1]);
        info.srcIP6[2]=(ctx->local_ip6[2]);
        info.srcIP6[3]=(ctx->local_ip6[3]);

        info.dstIP6[0]=(ctx->remote_ip6[0]);
        info.dstIP6[1]=(ctx->remote_ip6[1]);
        info.dstIP6[2]=(ctx->remote_ip6[2]);
        info.dstIP6[3]=(ctx->remote_ip6[3]);

        


        bpf_core_read_user(&info.dstIP6, sizeof(info.dstIP6), ctx->remote_ip6);
        bpf_core_read_user(&info.srcIP6, sizeof(info.srcIP6), ctx->local_ip6);


        bpf_core_read(info.dstIP6, 16, &ctx->remote_ip6[0]); // 16 байт == 4 * 4








