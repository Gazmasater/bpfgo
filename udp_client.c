#define _GNU_SOURCE
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

static void die(const char *msg) {
    perror(msg);
    _exit(1);
}

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <host> <port> [message]\n", argv[0]);
        return 1;
    }
    const char *host = argv[1];
    const char *port = argv[2];
    const char *msg  = (argc >= 4) ? argv[3] : "ping";

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    struct addrinfo *res = NULL;
    int rc = getaddrinfo(host, port, &hints, &res);
    if (rc != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rc));
        return 1;
    }

    int fd = -1;
    struct addrinfo *it;
    for (it = res; it; it = it->ai_next) {
        fd = socket(it->ai_family, it->ai_socktype, it->ai_protocol);
        if (fd >= 0) break;
    }
    if (fd < 0) die("socket");

    // timeout on recvfrom
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    // sendto (UDP)
    ssize_t s = sendto(fd, msg, strlen(msg), 0, it->ai_addr, it->ai_addrlen);
    if (s < 0) die("sendto");
    printf("sent %zd bytes to %s:%s\n", s, host, port);

    // recvfrom
    char buf[2048];
    struct sockaddr_storage peer;
    socklen_t peerlen = sizeof(peer);

    ssize_t n = recvfrom(fd, buf, sizeof(buf) - 1, 0,
                         (struct sockaddr *)&peer, &peerlen);
    if (n < 0) die("recvfrom");

    buf[n] = 0;
    printf("recv %zd bytes: '%s'\n", n, buf);

    close(fd);
    freeaddrinfo(res);
    return 0;
}