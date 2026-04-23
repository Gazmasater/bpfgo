#define _GNU_SOURCE
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char **argv) {
    const char *host = (argc > 1) ? argv[1] : "example.com";
    const char *port = (argc > 2) ? argv[2] : "443";

    struct addrinfo hints, *res = 0;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;      // проще для теста
    hints.ai_socktype = SOCK_STREAM;

    int rc = getaddrinfo(host, port, &hints, &res);
    if (rc != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rc));
        return 1;
    }

    int fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (fd < 0) { perror("socket"); return 1; }

    if (connect(fd, res->ai_addr, res->ai_addrlen) != 0) {
        perror("connect");
        close(fd);
        return 1;
    }

    // любой буфер, лишь бы sendmsg случился
    const char payload[] = "HELLO_FROM_SENDMSG\n";
    struct iovec iov = {
        .iov_base = (void*)payload,
        .iov_len  = sizeof(payload)-1
    };
    struct msghdr msg;
    memset(&msg, 0, sizeof(msg));
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    ssize_t n = sendmsg(fd, &msg, MSG_NOSIGNAL);
    if (n < 0) perror("sendmsg");

    // маленькая пауза, чтобы ты успевал дампнуть map
    usleep(200 * 1000);

    close(fd);
    freeaddrinfo(res);
    return 0;
}
