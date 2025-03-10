#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sock = 3; // Подставьте реальный файловый дескриптор уже подключенного сокета

    struct sockaddr_in local_addr;
    socklen_t addr_len = sizeof(local_addr);

    // Получаем локальный IP-адрес и порт для уже установленного соединения
    if (getsockname(sock, (struct sockaddr *)&local_addr, &addr_len) == -1) {
        perror("getsockname failed");
        return -1;
    }

    // Преобразуем IP-адрес в строковый формат
    char local_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &local_addr.sin_addr, local_ip, sizeof(local_ip));

    // Выводим локальный IP-адрес и порт
    printf("Local IP address: %s\n", local_ip);
    printf("Local port: %d\n", ntohs(local_addr.sin_port));

    return 0;
}