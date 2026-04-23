#define _GNU_SOURCE
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip6.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static void die(const char *m) { perror(m); exit(1); }

static void ip6_to_str(const struct in6_addr *a, char *out, size_t n) {
    if (!inet_ntop(AF_INET6, a, out, n)) strncpy(out, "?", n);
}

int main(int argc, char **argv) {
    int port = 9997;
    if (argc > 1) port = atoi(argv[1]);

    int s = socket(AF_INET6, SOCK_DGRAM, 0);
    if (s < 0) die("socket");

    int one = 1;
    // получать IPV6_PKTINFO на recvmsg
    if (setsockopt(s, IPPROTO_IPV6, IPV6_RECVPKTINFO, &one, sizeof(one)) < 0)
        die("setsockopt(IPV6_RECVPKTINFO)");

    struct sockaddr_in6 sa = {0};
    sa.sin6_family = AF_INET6;
    sa.sin6_addr = in6addr_any;      // bind ::
    sa.sin6_port = htons((uint16_t)port);

    if (bind(s, (struct sockaddr*)&sa, sizeof(sa)) < 0) die("bind");

    fprintf(stderr, "udp6_pktinfo_server listening on [::]:%d\n", port);

    for (;;) {
        char buf[2048];
        struct iovec iov = {.iov_base = buf, .iov_len = sizeof(buf)};

        char cbuf[CMSG_SPACE(sizeof(struct in6_pktinfo))];
        memset(cbuf, 0, sizeof(cbuf));

        struct sockaddr_in6 peer = {0};
        struct msghdr msg = {0};
        msg.msg_name = &peer;
        msg.msg_namelen = sizeof(peer);
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;
        msg.msg_control = cbuf;
        msg.msg_controllen = sizeof(cbuf);

        ssize_t n = recvmsg(s, &msg, 0);
        if (n < 0) {
            if (errno == EINTR) continue;
            die("recvmsg");
        }

        // Extract local dst + ifindex from IPV6_PKTINFO
        struct in6_addr local = {0};
        unsigned ifidx = 0;

        for (struct cmsghdr *c = CMSG_FIRSTHDR(&msg); c; c = CMSG_NXTHDR(&msg, c)) {
            if (c->cmsg_level == IPPROTO_IPV6 && c->cmsg_type == IPV6_PKTINFO) {
                struct in6_pktinfo *pi = (struct in6_pktinfo*)CMSG_DATA(c);
                local = pi->ipi6_addr;
                ifidx = pi->ipi6_ifindex;
                break;
            }
        }

        char peer_ip[128], local_ip[128];
        ip6_to_str(&peer.sin6_addr, peer_ip, sizeof(peer_ip));
        ip6_to_str(&local, local_ip, sizeof(local_ip));

        fprintf(stderr, "recv %zdB from [%s]:%d local_dst=[%s] ifindex=%u\n",
                n, peer_ip, ntohs(peer.sin6_port), local_ip, ifidx);

        // echo back with sendmsg + IPV6_PKTINFO to set source and ifindex
        struct msghdr sm = {0};
        struct iovec siov = {.iov_base = buf, .iov_len = (size_t)n};

        char scbuf[CMSG_SPACE(sizeof(struct in6_pktinfo))];
        memset(scbuf, 0, sizeof(scbuf));

        sm.msg_name = &peer;
        sm.msg_namelen = sizeof(peer);
        sm.msg_iov = &siov;
        sm.msg_iovlen = 1;
        sm.msg_control = scbuf;
        sm.msg_controllen = sizeof(scbuf);

        struct cmsghdr *sc = CMSG_FIRSTHDR(&sm);
        sc->cmsg_level = IPPROTO_IPV6;
        sc->cmsg_type  = IPV6_PKTINFO;
        sc->cmsg_len   = CMSG_LEN(sizeof(struct in6_pktinfo));

        struct in6_pktinfo *spi = (struct in6_pktinfo*)CMSG_DATA(sc);
        memset(spi, 0, sizeof(*spi));
        spi->ipi6_addr = local;   // хотим такой src
        spi->ipi6_ifindex = ifidx; // важно для link-local

        ssize_t sn = sendmsg(s, &sm, 0);
        if (sn < 0) die("sendmsg");
    }
}