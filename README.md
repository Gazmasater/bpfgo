#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <fcntl.h>  // Для O_NONBLOCK
#include <sys/select.h>

#define PORT 8080

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    char buffer[256];

    // Создание сокета
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    // Настройка адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Привязка сокета к адресу
    if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    // Прослушивание входящих соединений
    if (listen(sockfd, 5) < 0) {
        perror("ERROR on listen");
        exit(1);
    }

    client_len = sizeof(client_addr);

    // Принятие соединения
    newsockfd = accept(sockfd, (struct sockaddr *) &client_addr, &client_len);
    if (newsockfd < 0) {
        perror("ERROR on accept");
        exit(1);
    }

    // Установка неблокирующего режима
    int flags = fcntl(newsockfd, F_GETFL, 0);
    fcntl(newsockfd, F_SETFL, flags | O_NONBLOCK);

    // Использование select для ожидания данных
    fd_set readfds;
    struct timeval tv;

    FD_ZERO(&readfds);
    FD_SET(newsockfd, &readfds);

    tv.tv_sec = 5;  // Таймаут на 5 секунд
    tv.tv_usec = 0;

    int activity = select(newsockfd + 1, &readfds, NULL, NULL, &tv);
    if (activity < 0) {
        perror("select error");
    } else if (activity == 0) {
        printf("Timeout, no data available\n");
    } else {
        if (FD_ISSET(newsockfd, &readfds)) {
            memset(buffer, 0, 256);
            if (read(newsockfd, buffer, 255) < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    printf("No data available, try again later.\n");
                } else {
                    perror("ERROR reading from socket");
                    exit(1);
                }
            }
            printf("Message from client: %s\n", buffer);
        }
    }

    // Закрытие сокетов
    close(newsockfd);
    close(sockfd);

    return 0;
}


sudo apt update

sudo apt install binutils

sudo apt install gdb


objdump --version
gdb --version



gcc -o server server.c

gcc -o server server.c -D_GNU_SOURCE


objdump -d server | grep accept4

gdb ./server

(gdb) break __sys_accept4
(gdb) run


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ ./server
ERROR reading from socket: Resource temporarily unavailable
