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

    struct sockaddr_in6 peer = {0};
    peer.sin6_family = AF_INET6;
    peer.sin6_addr = in6addr_loopback;
    peer.sin6_port = htons((uint16_t)port);
    if (connect(fd, (struct sockaddr *)&peer, sizeof(peer)) < 0) {
        perror("connect"); return 1;
    }

    struct mmsghdr out[MAX] = {0}, in[MAX] = {0};
    struct iovec oiov[MAX] = {0}, iov[MAX] = {0};
    char msg[MAX][32], buf[MAX][64];
    for (int i = 0; i < MAX; i++) {
        snprintf(msg[i], sizeof(msg[i]), "v6m%02d", i);
        oiov[i].iov_base = msg[i];
        oiov[i].iov_len = strlen(msg[i]);
        out[i].msg_hdr.msg_iov = &oiov[i];
        out[i].msg_hdr.msg_iovlen = 1;
        iov[i].iov_base = buf[i];
        iov[i].iov_len = sizeof(buf[i]);
        in[i].msg_hdr.msg_iov = &iov[i];
        in[i].msg_hdr.msg_iovlen = 1;
    }

    int sent = sendmmsg(fd, out, MAX, 0);
    if (sent < 0) { perror("sendmmsg"); return 1; }
    int received = recvmmsg(fd, in, MAX, 0, NULL);
    if (received < 0) { perror("recvmmsg"); return 1; }
    printf("ipv6 client sent=%d received=%d\n", sent, received);
    return received == MAX ? 0 : 1;
}
