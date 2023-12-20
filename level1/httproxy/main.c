#include <stdio.h>
#include <stdlib.h>

#include "http_server.h"

int main(void) {
    HttpServer* http_server = create_http_server(8080);

    if (http_server->listen_and_serve(http_server) != EXIT_SUCCESS) {
        perror("http_server failed");
        http_server->destroy(http_server);
        return EXIT_FAILURE;        
    }

    http_server->destroy(http_server);

    return EXIT_SUCCESS;
}
d