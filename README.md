// Копирование IPv6-адреса в src_ip6
for (int i = 0; i < 8; i++) {
    uint16_t addr = addr6.sin6_addr.in6_u.u6_addr16[i];
    // Копируем старший байт в src_ip6[2*i] и младший байт в src_ip6[2*i + 1]
    conn_info->src_ip6[2*i] = (addr >> 8) & 0xFF;  // старший байт
    conn_info->src_ip6[2*i + 1] = addr & 0xFF;     // младший байт
}

// Выводим src_ip6 для проверки
bpf_printk("EXIT_accept4 src_ip6 for PID=%d: ", pid);
for (int i = 0; i < 8; i++) {
    uint16_t addr = (conn_info->src_ip6[2*i] << 8) | conn_info->src_ip6[2*i + 1];
    // Печатаем старший и младший байт, как раньше
    bpf_printk("%02x%02x ", (addr >> 8) & 0xFF, addr & 0xFF);
}
bpf_printk("\n");
