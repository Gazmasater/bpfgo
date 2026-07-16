#define _GNU_SOURCE

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <net/if.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static void die(const char *m) { perror(m); exit(1); }

int main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr, "usage: %s <dst_ipv6%%if> <port> <msg>\n", argv[0]);
        fprintf(stderr, "example: %s fe80::1234%%enp0s3 9997 ping\n", argv[0]);
        return 2;
    }
    const char *dst = argv[1];
    int port = atoi(argv[2]);
    const char *msg = argv[3];

    int s = socket(AF_INET6, SOCK_DGRAM, 0);
    if (s < 0) die("socket");

    struct sockaddr_in6 to = {0};
    to.sin6_family = AF_INET6;
    to.sin6_port = htons((uint16_t)port);

    // inet_pton не понимает %iface, поэтому режем scope вручную
    char addrbuf[256];
    strncpy(addrbuf, dst, sizeof(addrbuf)-1);
    addrbuf[sizeof(addrbuf)-1] = 0;

    char *pct = strchr(addrbuf, '%');
    if (pct) *pct = 0;

    if (inet_pton(AF_INET6, addrbuf, &to.sin6_addr) != 1) {
        fprintf(stderr, "bad ipv6: %s\n", addrbuf);
        return 2;
    }

    // scope id обязателен для fe80::
    if (strchr(dst, '%')) {
        const char *scope = strchr(dst, '%') + 1;
        unsigned ifidx = if_nametoindex(scope);
        if (ifidx == 0) {
            fprintf(stderr, "bad iface in scope: %s\n", scope);
            return 2;
        }
        to.sin6_scope_id = ifidx;
    }

    if (sendto(s, msg, strlen(msg), 0, (struct sockaddr*)&to, sizeof(to)) < 0)
        die("sendto");

    char buf[2048];
    struct sockaddr_in6 from = {0};
    socklen_t fl = sizeof(from);
    ssize_t n = recvfrom(s, buf, sizeof(buf)-1, 0, (struct sockaddr*)&from, &fl);
    if (n < 0) die("recvfrom");
    buf[n] = 0;

    char from_ip[128];
    if (!inet_ntop(AF_INET6, &from.sin6_addr, from_ip, sizeof(from_ip)))
        strncpy(from_ip, "?", sizeof(from_ip));

    printf("reply %zdB from [%s]:%d: %s\n", n, from_ip, ntohs(from.sin6_port), buf);
    close(s);
    return 0;
}