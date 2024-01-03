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
    void (*interrupt)(struct  HttpServer* self);
    pthread_t main_thread_id;
} HttpServer;

HttpServer* create_http_server(int port);
