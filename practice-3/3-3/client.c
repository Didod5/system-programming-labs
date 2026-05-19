#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8081
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;

    char buffer[BUFFER_SIZE];
    int length;

    // Создание сокета
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Настройка адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // IP сервера
    if (inet_pton(AF_INET, "127.0.0.1",
                  &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Подключение к серверу
    if (connect(sockfd,
                (struct sockaddr *)&server_addr,
                sizeof(server_addr)) < 0) {
        perror("connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Enter string: ");
    fgets(buffer, BUFFER_SIZE, stdin);

    // Удаление символа '\n'
    buffer[strcspn(buffer, "\n")] = '\0';

    // Отправка строки серверу
    if (send(sockfd, buffer, strlen(buffer), 0) < 0) {
        perror("send");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Получение результата
    ssize_t bytes_received = recv(sockfd,
                                  &length,
                                  sizeof(length),
                                  0);

    if (bytes_received < 0) {
        perror("recv");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("String length: %d\n", length);

    close(sockfd);

    return 0;
}