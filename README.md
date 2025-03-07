gcc udp_server.c -o udp_server

u8 *ip6 = (u8 *) addr_in6.sin6_addr.s6_addr;
u16 port = bpf_ntohs(addr_in6.sin6_port);

struct trace_info info = {};
info.pid = pid;
__builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));

info.pid = conn_info->pid;

// Для IPv6 записываем в info.dst_ip только первые 4 байта
info.dst_ip = *(u32 *)ip6;  // Сохраняем только первые 4 байта

// Для сохранения полного IPv6-адреса используем отдельное поле
memcpy(info.dst_ip_full, ip6, 16);  // Сохраняем весь 16-байтовый IPv6-адрес

info.dport = port;

bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

// Логирование для IPv6
bpf_printk("IPv6 detected: port %d, address %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x\n",
    port,
    ip6[0], ip6[1], ip6[2], ip6[3], ip6[4], ip6[5], ip6[6], ip6[7],
    ip6[8], ip6[9], ip6[10], ip6[11], ip6[12], ip6[13], ip6[14], ip6[15]);
