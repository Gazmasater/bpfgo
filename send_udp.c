//go:build ignore

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(33333);
    inet_pton(AF_INET, "127.0.0.1", &dest_addr.sin_addr);

    const char *msg = "Hello via sendmsg";
    struct iovec iov = {
        .iov_base = (void *)msg,
        .iov_len = strlen(msg)
    };

    struct msghdr msg_hdr = {
        .msg_name = &dest_addr,
        .msg_namelen = sizeof(dest_addr),
        .msg_iov = &iov,
        .msg_iovlen = 1
    };

    for (int i = 0; i < 1000; i++) {
        if (sendmsg(sock, &msg_hdr, 0) < 0) {
            perror("sendmsg");
            break;
        }
    }

    close(sock);
    return 0;
}