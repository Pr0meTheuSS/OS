#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_BUFFER_SIZE 1024
#define SERVER_PORT 8080
#define BACKLOG 5

int main() {
    int serverSocket, clientSocket, recvBytes;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    char buffer[MAX_BUFFER_SIZE];

    // Создание TCP сокета
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Ошибка при создании сокета");
        exit(1);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Ошибка при привязке сокета");
        exit(1);
    }

    if (listen(serverSocket, BACKLOG) < 0) {
        perror("Ошибка при прослушивании порта");
        exit(1);
    }

    printf("Сервер запущен и прослушивает порт %d...\n", SERVER_PORT);

    // Принятие входящего подключения от клиента
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (clientSocket < 0) {
        perror("Ошибка при принятии подключения");
        exit(1);
    }

    while (1) {
        // Получение данных от клиента
        recvBytes = read(clientSocket, buffer, MAX_BUFFER_SIZE - 1);
        if (recvBytes < 0) {
            perror("Ошибка при получении данных");
            exit(1);
        } else if (recvBytes == 0) {
            printf("Клиент отключился\n");
            break;
        }

        buffer[recvBytes] = '\0';
        printf("Получено от клиента: %s\n", buffer);

        // Отправка данных обратно клиенту
        if (write(clientSocket, buffer, strlen(buffer)) < 0) {
            perror("Ошибка при отправке данных");
            exit(1);
        }
    }

    // Закрытие сокетов
    close(clientSocket);
    close(serverSocket);

    return 0;
}

