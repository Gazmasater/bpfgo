
https://platform-balun.ru/teach/control/stream/view/id/930914027
user: mat3x@mail.ru
pass: Vkalashnikov00f!

__________________________________________________
GIT

git reset --hard HEAD
____________________________________________________



bpf2go -output-dir . -tags linux -type trace_info -go-package=main -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)



sudo nft add table ip6 test
sudo nft add chain ip6 test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule ip6 test prerouting exthdr type routing accept

chain prerouting {
    type nat hook prerouting priority 0;
}
sudo nft add rule ip test prerouting ip daddr 192.168.1.0/24 accept




sudo nft add table inet test
sudo nft add chain inet test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule inet test prerouting exthdr dst exists accept


sudo nft list ruleset

table inet test {
        chain prerouting {
                type filter hook prerouting priority filter; policy accept;
                exthdr dst exists accept
        }
}


sudo nft add rule ip6 test prerouting exthdr dst exists accept
sudo nft add rule ip6 test prerouting exthdr hopopts exists accept
sudo nft add rule ip6 test prerouting exthdr frag exists accept
sudo nft add rule ip6 test prerouting exthdr routing exists accept



git rebase -i db77c5b 
Первый из них оставляешь как pick, остальные 9 меняешь на squash (или s):

git rebase --abort


git push -u origin trace_core1 --force







_______________________________________________________________________________________________


go tool pprof http://localhost:6060/debug/pprof/profile?seconds=30
go tool pprof http://localhost:6060/debug/pprof/heap



"net/http"      // добавлено для pprof
	_ "net/http/pprof" // регистрирует pprof-эндпоинты

     _ "net/http/pprof" // blank-import: регистрирует pprof-хэндлеры в DefaultServeMux



git branch -m old-branch new-branch
git push origin -u new-branch


git checkout main
git pull origin main


git checkout ProcNet_monitor
git merge main

git push --force-with-lease origin ProcNet_monitor





// recvmsg_test.c
#define _GNU_SOURCE
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>   // atoi

static pid_t gettid_linux(void) {
    return (pid_t)syscall(SYS_gettid);
}

int main(int argc, char **argv) {
    int port = 9999;
    if (argc > 1) port = atoi(argv[1]);

    pid_t pid = getpid();
    pid_t tid = gettid_linux();

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) { perror("socket"); return 1; }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons((uint16_t)port);

    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return 1;
    }

    printf("[pid=%d tid=%d] recvmsg UDP server listening on 0.0.0.0:%d\n",
           (int)pid, (int)tid, port);

    for (;;) {
        char buf[2048];

        struct sockaddr_storage peer;
        socklen_t peerlen = sizeof(peer);

        struct iovec iov = {
            .iov_base = buf,
            .iov_len  = sizeof(buf),
        };

        struct msghdr msg;
        memset(&msg, 0, sizeof(msg));
        msg.msg_name = &peer;
        msg.msg_namelen = peerlen;
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;

        ssize_t n = recvmsg(fd, &msg, 0);
        if (n < 0) {
            if (errno == EINTR) continue;
            perror("recvmsg");
            break;
        }

        // распечатаем src ip:port
        char ipstr[INET6_ADDRSTRLEN] = {0};
        int pport = 0;

        if (peer.ss_family == AF_INET) {
            struct sockaddr_in *sin = (struct sockaddr_in*)&peer;
            inet_ntop(AF_INET, &sin->sin_addr, ipstr, sizeof(ipstr));
            pport = ntohs(sin->sin_port);
        } else if (peer.ss_family == AF_INET6) {
            struct sockaddr_in6 *sin6 = (struct sockaddr_in6*)&peer;
            inet_ntop(AF_INET6, &sin6->sin6_addr, ipstr, sizeof(ipstr));
            pport = ntohs(sin6->sin6_port);
        } else {
            strcpy(ipstr, "unknown_family");
        }

        printf("[pid=%d tid=%d] got %zd bytes from %s:%d: %.*s\n",
               (int)pid, (int)tid, n, ipstr, pport, (int)n, buf);
        fflush(stdout);
    }

    close(fd);
    return 0;
}




// send_udp.c
#define _GNU_SOURCE
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>   // atoi

static pid_t gettid_linux(void) {
    return (pid_t)syscall(SYS_gettid);
}

int main(int argc, char **argv) {
    const char *ip = "127.0.0.1";
    int port = 9999;
    const char *msg = "hello";

    if (argc > 1) ip = argv[1];
    if (argc > 2) port = atoi(argv[2]);
    if (argc > 3) msg = argv[3];

    pid_t pid = getpid();
    pid_t tid = gettid_linux();

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) { perror("socket"); return 1; }

    struct sockaddr_in dst;
    memset(&dst, 0, sizeof(dst));
    dst.sin_family = AF_INET;
    dst.sin_port = htons((uint16_t)port);
    inet_pton(AF_INET, ip, &dst.sin_addr);

    ssize_t n = sendto(fd, msg, strlen(msg), 0, (struct sockaddr*)&dst, sizeof(dst));
    if (n < 0) { perror("sendto"); return 1; }

    printf("[pid=%d tid=%d] sent %zd bytes to %s:%d\n",
           (int)pid, (int)tid, n, ip, port);

    close(fd);
    return 0;
}



gcc -O2 -Wall -o recvmsg_test recvmsg_test.c
gcc -O2 -Wall -o send_udp send_udp.c


./recvmsg_test 9999
./send_udp 127.0.0.1 9999 "ping1"


#include <stdlib.h>




RECVMSG PID=12435 NAME=recvmsg_test   UDP://localhost[127.0.0.1:56217]->localhost[127.0.0.1:9999]
RECVMSG PID=12515  NAME=recvmsg_test   UDP://localhost[127.0.0.1:56217]<-localhost[127.0.0.1:9999]


[pid=12435 tid=12435] recvmsg UDP server listening on 0.0.0.0:9999
[pid=12435 tid=12435] got 4 bytes from 127.0.0.1:39439: ping
[pid=12435 tid=12435] got 4 bytes from 127.0.0.1:56217: ping



lev@lev-VirtualBox:~/bpfgo$ ./send_udp 127.0.0.1 9999 "ping"
[pid=12470 tid=12470] sent 4 bytes to 127.0.0.1:9999
lev@lev-VirtualBox:~/bpfgo$ ./send_udp 127.0.0.1 9999 "ping"
[pid=12515 tid=12515] sent 4 bytes to 127.0.0.1:9999
lev@lev-VirtualBox:~/bpfgo$ 



strace -f -e trace=sendto,sendmsg ./send_udp 127.0.0.1 9999 ping












