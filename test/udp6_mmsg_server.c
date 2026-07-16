#define _GNU_SOURCE
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <unistd.h>

#define MAX 16

int main(int argc, char **argv) {
    int port = argc > 1 ? atoi(argv[1]) : 9996;
    int fd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (fd < 0) { perror("socket"); return 1; }

    struct sockaddr_in6 sa = {0};
    sa.sin6_family = AF_INET6;
    sa.sin6_addr = in6addr_any;
    sa.sin6_port = htons((uint16_t)port);
    if (bind(fd, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
        perror("bind"); return 1;
    }

    for (;;) {
        struct mmsghdr in[MAX] = {0}, out[MAX] = {0};
        struct iovec iov[MAX] = {0}, oiov[MAX] = {0};
        struct sockaddr_in6 peer[MAX] = {0};
        char buf[MAX][256];

        for (int i = 0; i < MAX; i++) {
            iov[i].iov_base = buf[i];
            iov[i].iov_len = sizeof(buf[i]);
            in[i].msg_hdr.msg_name = &peer[i];
            in[i].msg_hdr.msg_namelen = sizeof(peer[i]);
            in[i].msg_hdr.msg_iov = &iov[i];
            in[i].msg_hdr.msg_iovlen = 1;
        }

        int n = recvmmsg(fd, in, MAX, 0, NULL);
        if (n < 0) { if (errno == EINTR) continue; perror("recvmmsg"); return 1; }

        for (int i = 0; i < n; i++) {
            oiov[i].iov_base = buf[i];
            oiov[i].iov_len = in[i].msg_len;
            out[i].msg_hdr.msg_name = &peer[i];
            out[i].msg_hdr.msg_namelen = sizeof(peer[i]);
            out[i].msg_hdr.msg_iov = &oiov[i];
            out[i].msg_hdr.msg_iovlen = 1;
        }
        if (sendmmsg(fd, out, n, 0) < 0) { perror("sendmmsg"); return 1; }
    }
}
