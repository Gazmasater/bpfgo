#define _GNU_SOURCE
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <time.h>
#include <unistd.h>

#ifndef MAX
#define MAX 16
#endif

int main() {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) { perror("socket"); return 1; }

    struct sockaddr_in sa = {0};
    sa.sin_family = AF_INET;
    sa.sin_port = htons(9999);
    sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (bind(fd, (struct sockaddr*)&sa, sizeof(sa)) < 0) {
        perror("bind");
        return 1;
    }

    printf("udp_mmsg_server: 127.0.0.1:9999 recvmmsg->sendmmsg echo\n");

    for (;;) {
        struct mmsghdr in[MAX];
        struct iovec   iov[MAX];
        struct sockaddr_in peer[MAX];
        char bufs[MAX][256];

        memset(in, 0, sizeof(in));
        memset(peer, 0, sizeof(peer));

        for (int i = 0; i < MAX; i++) {
            iov[i].iov_base = bufs[i];
            iov[i].iov_len  = sizeof(bufs[i]);
            in[i].msg_hdr.msg_iov = &iov[i];
            in[i].msg_hdr.msg_iovlen = 1;
            in[i].msg_hdr.msg_name = &peer[i];
            in[i].msg_hdr.msg_namelen = sizeof(peer[i]);
        }

        int n = recvmmsg(fd, in, MAX, 0, NULL);
        if (n < 0) {
            if (errno == EINTR) continue;
            perror("recvmmsg");
            return 1;
        }

        // готовим outgoing пачку
        struct mmsghdr out[MAX];
        struct iovec   oiov[MAX];
        memset(out, 0, sizeof(out));
        memset(oiov, 0, sizeof(oiov));

        for (int i = 0; i < n; i++) {
            oiov[i].iov_base = bufs[i];
            oiov[i].iov_len  = in[i].msg_len;
            out[i].msg_hdr.msg_iov = &oiov[i];
            out[i].msg_hdr.msg_iovlen = 1;
            out[i].msg_hdr.msg_name = &peer[i];
            out[i].msg_hdr.msg_namelen = sizeof(peer[i]);
        }

        int s = sendmmsg(fd, out, n, 0);
        if (s < 0) {
            perror("sendmmsg");
            return 1;
        }
    }
}