#include <arpa/inet.h>
#include <linux/futex.h>
#include <netdb.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "http_server.h"

int conn2host(const char *hostname, char* port) {
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
    // free(client_socket_ptr);

    char *buffer = (char *)malloc(BUFFER_SIZE);
    if (!buffer) {
        perror("Error allocating memory");
        close(client_socket);
        pthread_exit(NULL);
    }

    ssize_t total_bytes_received = 0;
    ssize_t bytes_received;
    ssize_t buffer_size = BUFFER_SIZE;

    while ((bytes_received = recv(client_socket, buffer + total_bytes_received, buffer_size - total_bytes_received, 0)) > 0) {
        total_bytes_received += bytes_received;

        if (strstr(buffer, "\r\n\r\n") != NULL) {
            break;
        }

        if (total_bytes_received == buffer_size) {
            buffer_size *= 2;
            buffer = (char *)realloc(buffer, buffer_size);
            if (!buffer) {
                perror("Error reallocating memory");
                close(client_socket);
                pthread_exit(NULL);
            }
        }
    }

    if (bytes_received < 0) {
        perror("Error receiving data");
        close(client_socket);
        free(buffer);
        pthread_exit(NULL);
    }

    printf("Received HTTP request:\n%.*s\n", (int)total_bytes_received, buffer);

    char requested_url[BUFFER_SIZE];
    if (sscanf(buffer, "GET %s HTTP", requested_url) != 1) {
        if (sscanf(buffer, "CONNECT %s HTTP", requested_url) != 1) {
            perror("Error parsing HTTP request");
            close(client_socket);
            free(buffer);
            pthread_exit(NULL);
        }
    }

    printf("Requested URL: %s\n", requested_url);
    char host[BUFFER_SIZE];
    int port;

    sscanf(requested_url, "http://%[^:/]:%d", host, &port);
    printf("HOST: %s \n", host);

    int server_socket = conn2host(host, "80");
    if (server_socket == -1) {
        perror("Error connecting to host");
        free(buffer);
        pthread_exit(NULL);
    }

    printf("Open server socket %d\n", server_socket);

    ssize_t bytes_sent = send(server_socket, buffer, total_bytes_received, 0);
    if (bytes_sent < 0) {
        perror("Error sending data to server");
        close(client_socket);
        close(server_socket);
        free(buffer);
        pthread_exit(NULL);
    }
    printf("sent %s\n", buffer);

    while ((bytes_received = recv(server_socket, buffer, buffer_size, 0)) > 0) {
        printf("Received response chunk:\n%.*s\n", (int)bytes_received, buffer);

        bytes_sent = send(client_socket, buffer, bytes_received, 0);
        if (bytes_sent < 0) {
            perror("Error sending data to client");
            close(client_socket);
            close(server_socket);
            free(buffer);
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

    free(buffer);
    pthread_exit(NULL);
}

static char is_interrupted = 0;

void interrupt_server(HttpServer* self) {
    pthread_cancel(self->main_thread_id);
    is_interrupted = 1;
}

int futex_variable = 0;
void handleCtrlC(int signum) {
    printf("\nReceived Ctrl+C signal. Cleaning up...\n");    
    is_interrupted = 1;
    futex_variable = 1;
    syscall(SYS_futex, &futex_variable, FUTEX_WAKE, 1, NULL, NULL, 0);
}

void close_server_socket(void* arg) {
    close(*(int*)arg);
    printf("Server socket is closed");
}

void* mthread_http_listen_and_serv_impl(void* args) {
    struct sigaction sa;
    sa.sa_handler = handleCtrlC;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Error setting up signal handler");
        exit(EXIT_FAILURE);
    }

    HttpServer* self = (HttpServer*)args;
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    pthread_t tid;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Error creating socket");
        pthread_exit(NULL);
    }

    pthread_cleanup_push(close_server_socket, &server_socket);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(self->port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding socket");
        printf("Server socket: %d, server_addr: %d, ", server_socket, server_addr.sin_port);
        close(server_socket);
        pthread_exit(NULL);
    }

    if (listen(server_socket, 8) < 0) {
        perror("Error listening for connections");
        close(server_socket);
        pthread_exit(NULL);
    }

    printf("Proxy server listening on port %d...\n", self->port);

    while (!is_interrupted) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            perror("Error accepting connection");
            continue;
        }



        if (pthread_create(&tid, NULL, handle_request, (void *)&client_socket) != 0) {
            perror("Error creating thread");
            close(client_socket);
            pthread_exit(NULL);
        }

        pthread_detach(tid);
    }

    // close(server_socket);
    pthread_cleanup_pop(1);
    pthread_exit(NULL);
}

int init_and_run_listen_and_serve(HttpServer* self) {
    if (pthread_create(&self->main_thread_id, NULL, mthread_http_listen_and_serv_impl, self) != 0) {
        perror("Error creating thread");
        return EXIT_FAILURE;
    }

    while (!is_interrupted) {
        syscall(SYS_futex, &futex_variable, FUTEX_WAIT, 0, NULL, NULL, 0);
    }

    interrupt_server(self);
    pthread_join(self->main_thread_id, NULL);
    return EXIT_SUCCESS;
}

void destroy_impl(HttpServer* server) {
    free(server);
}

HttpServer* create_http_server(int port) {
    HttpServer* new_http_server = (HttpServer*)malloc(sizeof(HttpServer));
    new_http_server->port = port;
    new_http_server->listen_and_serve = init_and_run_listen_and_serve;
    new_http_server->destroy = destroy_impl;
    new_http_server->interrupt = interrupt_server;
    return new_http_server;
}
