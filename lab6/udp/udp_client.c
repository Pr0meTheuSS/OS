#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345

int main() {
    int clientSocket, recvBytes;
    struct sockaddr_in serverAddr;
    socklen_t serverAddrLen = sizeof(serverAddr);
    char buffer[MAX_BUFFER_SIZE];

    // Создание UDP сокета
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket < 0) {
        perror("Socket init error");
        exit(1);
    }

    // Настройка адреса сервера
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    if (inet_aton(SERVER_IP, &serverAddr.sin_addr) == 0) {
        perror("Incorrect ip");
        exit(1);
    }

    while (1) {
        printf("send to:\n");
        fgets(buffer, MAX_BUFFER_SIZE, stdin);

        if (sendto(clientSocket, buffer, strlen(buffer), 0,
                   (struct sockaddr*)&serverAddr, serverAddrLen) < 0) {
            perror("send error");
            exit(1);
        }

        recvBytes = recvfrom(clientSocket, buffer, MAX_BUFFER_SIZE - 1, 0,
                             (struct sockaddr*)&serverAddr, &serverAddrLen);
        if (recvBytes < 0) {
            perror("recvBytes error");
            exit(1);
        }

        buffer[recvBytes] = '\0';
        printf("recived: %s\n", buffer);
    }

    close(clientSocket);

    return 0;
}

