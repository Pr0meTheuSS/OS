#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_BUFFER_SIZE 1024
#define SERVER_PORT 12345

int main() {
    int serverSocket, recvBytes;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    char buffer[MAX_BUFFER_SIZE];

    // Создание UDP сокета
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket < 0) {
        perror("Ошибка при создании сокета");
        exit(1);
    }

    // Настройка адреса сервера
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Привязка сокета к адресу сервера
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Ошибка при привязке сокета");
        exit(1);
    }

    printf("Server is listening %d...\n", SERVER_PORT);

    while (1) {
        // Получение данных от клиента
        recvBytes = recvfrom(serverSocket, buffer, MAX_BUFFER_SIZE - 1, 0,
                             (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (recvBytes < 0) {
            perror("recvBytes error");
            exit(1);
        }

        buffer[recvBytes] = '\0';
        printf("Recived: %s\n", buffer);

        // Отправка данных обратно клиенту
        if (sendto(serverSocket, buffer, strlen(buffer), 0,
                   (struct sockaddr*)&clientAddr, clientAddrLen) < 0) {
            perror("Send error");
            exit(1);
        }
    }

    // Закрытие сокета
    close(serverSocket);

    return 0;
}

