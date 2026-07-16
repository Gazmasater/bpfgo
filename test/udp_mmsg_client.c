#define _GNU_SOURCE
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <unistd.h>

#ifndef MAX
#define MAX 16
#endif

int main() {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) { perror("socket"); return 1; }

    struct sockaddr_in dst = {0};
    dst.sin_family = AF_INET;
    dst.sin_port = htons(9999);
    dst.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (connect(fd, (struct sockaddr*)&dst, sizeof(dst)) < 0) {
        perror("connect");
        return 1;
    }

    struct mmsghdr out[MAX];
    struct iovec   oiov[MAX];
    char msg[MAX][32];

    memset(out, 0, sizeof(out));
    for (int i = 0; i < MAX; i++) {
        snprintf(msg[i], sizeof(msg[i]), "m%02d", i);
        oiov[i].iov_base = msg[i];
        oiov[i].iov_len  = strlen(msg[i]);
        out[i].msg_hdr.msg_iov = &oiov[i];
        out[i].msg_hdr.msg_iovlen = 1;
    }

    int s = sendmmsg(fd, out, MAX, 0);
    if (s < 0) { perror("sendmmsg"); return 1; }

    struct mmsghdr in[MAX];
    struct iovec   iov[MAX];
    char buf[MAX][64];
    memset(in, 0, sizeof(in));

    for (int i = 0; i < MAX; i++) {
        iov[i].iov_base = buf[i];
        iov[i].iov_len  = sizeof(buf[i]);
        in[i].msg_hdr.msg_iov = &iov[i];
        in[i].msg_hdr.msg_iovlen = 1;
    }

    int n = recvmmsg(fd, in, MAX, 0, NULL);
    if (n < 0) { perror("recvmmsg"); return 1; }

    printf("client got %d msgs\n", n);
    for (int i = 0; i < n; i++) {
        printf("  %.*s\n", (int)in[i].msg_len, buf[i]);
    }

    return 0;
}