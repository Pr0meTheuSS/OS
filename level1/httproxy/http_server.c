#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "http_server.h"

int conn2host(const char *hostname) {
    const char *port = "80";

    struct addrinfo hints, *result, *rp;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    int status = getaddrinfo(hostname, port, &hints, &result);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return EXIT_FAILURE;
    }

    int client_socket = -1;
    for (rp = result; rp; rp = rp->ai_next) {
        client_socket = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (client_socket == -1) {
            continue;
        }

        if (connect(client_socket, rp->ai_addr, rp->ai_addrlen) != -1) {
            break;
        }

        close(client_socket);
    }

    freeaddrinfo(result);

    return client_socket;
}

void *handle_request(void *client_socket_ptr) {
    int client_socket = *((int *)client_socket_ptr);
    free(client_socket_ptr);

    char buffer[BUFFER_SIZE];
    ssize_t bytes_received, bytes_sent;

    bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received < 0) {
        perror("Error receiving data");
        close(client_socket);
        pthread_exit(NULL);
    }

    printf("Received HTTP request:\n%.*s\n", (int)bytes_received, buffer);

    char requested_url[BUFFER_SIZE];
    // TODO: move to parse function.
    if (sscanf(buffer, "GET %s HTTP", requested_url) != 1) {
        if (sscanf(buffer, "CONNECT %s HTTP", requested_url) != 1) {
            perror("Error parsing HTTP request");
            close(client_socket);
            pthread_exit(NULL);
        }        
    }

    printf("Requested URL: %s\n", requested_url);
    char host[BUFFER_SIZE];
    int port;

    sscanf(requested_url, "http://%[^:/]:%d", host, &port);
    printf("HOST: %s \n", host);

    int server_socket = conn2host(host);
    if (server_socket == -1) {
        perror("server_socket from connection to host\n");
        return NULL;
    }

    printf("Open server socket %d\n", server_socket);

    bytes_sent = send(server_socket, buffer, bytes_received, 0);
    if (bytes_sent < 0) {
        perror("Error sending data to server");
        close(client_socket);
        close(server_socket);
        pthread_exit(NULL);
    }
    printf("sent %s\n", buffer);

    // Receive and log the server's response
    while ((bytes_received = recv(server_socket, buffer, sizeof(buffer), 0)) > 0) {
        printf("Received response chunk:\n%.*s\n", (int)bytes_received, buffer);

        // Send the response chunk back to the client
        bytes_sent = send(client_socket, buffer, bytes_received, 0);
        if (bytes_sent < 0) {
            perror("Error sending data to client");
            close(client_socket);
            close(server_socket);
            pthread_exit(NULL);
        }
    }

    if (bytes_received < 0) {
        perror("Error receiving data from server");
    }

    close(client_socket);
    close(server_socket);

    printf("^^^^^^^^^^^^^^^^ Connection for this request is closed ^^^^^^^^^^^^^^^^\n");
    printf("||||||||||||||||                                       ||||||||||||||||\n");

    pthread_exit(NULL);
}

static char is_interrupted = 0;
int mthread_http_listen_and_serv_impl(HttpServer* self) {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    pthread_t tid;

    // Create a socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Error creating socket");
        return EXIT_FAILURE;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(self->port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the specified address and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding socket");
        printf("Server socket: %d, server_addr: %d, ", server_socket, server_addr.sin_port);
        close(server_socket);
        return EXIT_FAILURE;
    }

    // Listen for incoming connections
    if (listen(server_socket, 10) < 0) {
        perror("Error listening for connections");
        close(server_socket);
        return EXIT_FAILURE;
    }

    printf("Proxy server listening on port %d...\n", self->port);

    while (!is_interrupted) {
        // Accept a client connection
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            perror("Error accepting connection");
            continue;
        }

        // Allocate memory for the client socket descriptor to pass to the thread
        int *client_socket_ptr = (int *)malloc(sizeof(int));
        *client_socket_ptr = client_socket;

        // Create a new thread to handle the request
        if (pthread_create(&tid, NULL, handle_request, (void *)client_socket_ptr) != 0) {
            perror("Error creating thread");
            close(client_socket);
            free(client_socket_ptr);
            return EXIT_FAILURE;
        }

        pthread_detach(tid);
    }

    close(server_socket);

    return 0;
}

void destroy_impl(HttpServer* server) {
    free(server);
}

HttpServer* create_http_server(int port) {
    HttpServer* new_http_server = (HttpServer*)malloc(sizeof(HttpServer));
    new_http_server->port = port;
    new_http_server->listen_and_serve = mthread_http_listen_and_serv_impl;
    new_http_server->destroy = destroy_impl;
}

