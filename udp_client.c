#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 33333
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Создаем сокет
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }

    // Задаем параметры адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Ошибка преобразования адреса");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Ввод сообщения и отправка
    printf("Введите сообщение для отправки: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';  // Убираем символ новой строки

    if (sendto(sockfd, buffer, strlen(buffer), 0, 
               (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Ошибка при отправке данных");
    } else {
        printf("Сообщение отправлено серверу %s:%d\n", SERVER_IP, SERVER_PORT);
    }

    close(sockfd);
    return 0;
}
