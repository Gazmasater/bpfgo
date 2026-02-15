#define _GNU_SOURCE
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static void die(const char *msg) {
    perror(msg);
    _exit(1);
}

int main(int argc, char **argv) {
    const char *host = (argc >= 2) ? argv[1] : "0.0.0.0";
    const char *port = (argc >= 3) ? argv[2] : "9999";

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;     // IPv4/IPv6
    hints.ai_socktype = SOCK_DGRAM;    // UDP
    hints.ai_flags    = AI_PASSIVE;    // for bind

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
        if (fd < 0) continue;

        int one = 1;
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

        if (bind(fd, it->ai_addr, it->ai_addrlen) == 0) {
            break; // OK
        }
        close(fd);
        fd = -1;
    }

    freeaddrinfo(res);
    if (fd < 0) die("bind/socket");

    printf("UDP server listening on %s:%s\n", host, port);

    for (;;) {
        char buf[2048];
        struct sockaddr_storage peer;
        socklen_t peerlen = sizeof(peer);

        ssize_t n = recvfrom(fd, buf, sizeof(buf) - 1, 0,
                             (struct sockaddr *)&peer, &peerlen);
        if (n < 0) {
            if (errno == EINTR) continue;
            die("recvfrom");
        }
        buf[n] = 0;

        char ip[INET6_ADDRSTRLEN];
        unsigned short p = 0;

        if (peer.ss_family == AF_INET) {
            struct sockaddr_in *sa = (struct sockaddr_in *)&peer;
            inet_ntop(AF_INET, &sa->sin_addr, ip, sizeof(ip));
            p = ntohs(sa->sin_port);
        } else if (peer.ss_family == AF_INET6) {
            struct sockaddr_in6 *sa6 = (struct sockaddr_in6 *)&peer;
            inet_ntop(AF_INET6, &sa6->sin6_addr, ip, sizeof(ip));
            p = ntohs(sa6->sin6_port);
        } else {
            strcpy(ip, "?");
        }

        printf("recvfrom %s:%u  bytes=%zd  data='%s'\n", ip, p, n, buf);

        // echo back
        ssize_t s = sendto(fd, buf, (size_t)n, 0,
                           (struct sockaddr *)&peer, peerlen);
        if (s < 0) die("sendto");
    }
}