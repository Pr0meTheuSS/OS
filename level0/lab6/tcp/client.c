#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int main(void) {
    int clientSocket, recvBytes;
    struct sockaddr_in serverAddr;
    char buffer[MAX_BUFFER_SIZE];

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Ошибка при создании сокета");
        exit(1);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    if (inet_aton(SERVER_IP, &serverAddr.sin_addr) == 0) {
        perror("Некорректный IP адрес сервера");
        exit(1);
    }

    // Установка соединения с сервером
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Ошибка при установке соединения");
        exit(1);
    }

    printf("Подключение к серверу %s:%d...\n",
           inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));

    while (1) {
        printf("Введите сообщение для отправки:\n");
        fgets(buffer, MAX_BUFFER_SIZE, stdin);

        if (write(clientSocket, buffer, strlen(buffer)) < 0) {
            perror("Ошибка при отправке данных");
            exit(1);
        }

        recvBytes = read(clientSocket, buffer, MAX_BUFFER_SIZE - 1);
        if (recvBytes < 0) {
            perror("Ошибка при получении данных");
            exit(1);
        } else if (recvBytes == 0) {
            printf("Сервер отключился\n");
            break;
        }

        buffer[recvBytes] = '\0';
        printf("Получено от сервера: %s\n", buffer);
    }

    // Закрытие сокета
    close(clientSocket);

    return 0;
}

