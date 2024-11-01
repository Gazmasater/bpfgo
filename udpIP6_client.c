//go:build ignore

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 33333
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in6 server_addr;
    char buffer[BUFFER_SIZE] = "Привет, сервер!";
    
    // Создаем сокет
    if ((sockfd = socket(AF_INET6, SOCK_DGRAM, 0)) < 0) {
        perror("Ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }

    // Задаем параметры адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = AF_INET6;
    inet_pton(AF_INET6, "::1", &server_addr.sin6_addr); // Локальный IPv6 адрес
    server_addr.sin6_port = htons(PORT);

    // Отправляем сообщение
    if (sendto(sockfd, buffer, strlen(buffer), 0, 
               (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Ошибка при отправке данных");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Сообщение отправлено на [::1]:%d: %s\n", PORT, buffer);

    close(sockfd);
    return 0;
}
