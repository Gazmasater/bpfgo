#define _GNU_SOURCE
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) { perror("socket"); return 1; }

    int one = 1;
    if (setsockopt(fd, IPPROTO_IP, IP_PKTINFO, &one, sizeof(one)) < 0) {
        perror("setsockopt(IP_PKTINFO)");
        return 1;
    }

    struct sockaddr_in sa = {0};
    sa.sin_family = AF_INET;
    sa.sin_port = htons(9999);
    sa.sin_addr.s_addr = htonl(INADDR_ANY); // 0.0.0.0
    if (bind(fd, (struct sockaddr*)&sa, sizeof(sa)) < 0) { perror("bind"); return 1; }

    printf("udp_pktinfo_server: 0.0.0.0:9999 recvmsg(IP_PKTINFO) echo\n");

    for (;;) {
        char buf[2048];
        char cbuf[CMSG_SPACE(sizeof(struct in_pktinfo))];
        struct iovec iov = {.iov_base = buf, .iov_len = sizeof(buf)};
        struct msghdr msg;
        struct sockaddr_in peer;
        memset(&msg, 0, sizeof(msg));
        memset(&peer, 0, sizeof(peer));
        memset(cbuf, 0, sizeof(cbuf));

        msg.msg_name = &peer;
        msg.msg_namelen = sizeof(peer);
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;
        msg.msg_control = cbuf;
        msg.msg_controllen = sizeof(cbuf);

        ssize_t n = recvmsg(fd, &msg, 0);
        if (n <= 0) continue;

        // echo back
        sendto(fd, buf, n, 0, (struct sockaddr*)&peer, sizeof(peer));
    }
}