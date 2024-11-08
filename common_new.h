#define AF_INET 2
#define AF_INET6 10


struct accept_args_t {
    struct sockaddr_in* addr;
    int fd;
};

struct addr_t {
    u32 ip;
    u16 port;
};



static __inline void parse_address(struct addr_t *result, struct accept_args_t* args) {

    result->ip = 0;
    result->port = 0;

    struct sockaddr_in sin = {};
    struct sockaddr_in6 sin6 = {};

    // Read the address based on the sa_family
    struct sockaddr* saddr = (struct sockaddr *) args->addr;
    sa_family_t address_family = 0;
    bpf_probe_read(&address_family, sizeof(address_family), &saddr->sa_family);

   // if (address_family == AF_INET) {
        bpf_probe_read(&sin, sizeof(sin), args->addr);
        

       result->ip = sin.sin_addr.s_addr;
        result->port = sin.sin_port;

 bpf_printk("parse_address:port = %d\n", sin.sin_port);

    // } else if (address_family == AF_INET6) {
    //     bpf_probe_read(&sin6, sizeof(sin6), args->addr);
    //     result->port = sin6.sin6_port;
    //     u8 ipv6_addr[16];
    //    bpf_probe_read(&ipv6_addr, sizeof(ipv6_addr), &sin6.sin6_addr);

    //     // Check if it's an IPv4-mapped IPv6 address (::ffff:0:0/96 prefix)
    //     if (ipv6_addr[0] == 0 && ipv6_addr[1] == 0 && ipv6_addr[2] == 0 && ipv6_addr[3] == 0 &&
    //         ipv6_addr[4] == 0 && ipv6_addr[5] == 0 && ipv6_addr[6] == 0 && ipv6_addr[7] == 0 &&
    //         ipv6_addr[8] == 0 && ipv6_addr[9] == 0 && ipv6_addr[10] == 0xff && ipv6_addr[11] == 0xff) {
    //         // Extract the IPv4 address from the last 4 bytes
    //         result->ip = *(u32 *)&ipv6_addr[12];
    //     }
    // }
}