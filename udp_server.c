#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(client_addr);

    // Создаем сокет
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }

    // Задаем параметры адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Привязываем сокет
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Ошибка при привязке сокета");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP-сервер запущен и слушает порт %d...\n", PORT);

    // Принимаем сообщения
    while (1) {
        int len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, 
                           (struct sockaddr*)&client_addr, &addr_len);
        if (len < 0) {
            perror("Ошибка при получении данных");
            continue;
        }
        buffer[len] = '\0';  // Завершаем строку
        printf("Получено сообщение: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}


