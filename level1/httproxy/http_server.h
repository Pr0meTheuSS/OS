#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 8192

typedef struct HttpServer {
    int (*listen_and_serve)(struct HttpServer* self);
    int port;
    void (*destroy)(struct HttpServer* self);
} HttpServer;

// int conn2host(const char *hostname);

// void *handle_request(void *client_socket_ptr);

// static char is_interrupted = 0;
// int mthread_http_listen_and_serv_impl(HttpServer* self);

// void destroy_impl(HttpServer* server);

HttpServer* create_http_server(int port);
