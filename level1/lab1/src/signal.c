#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>

void sighandler(int) {
    printf("Handling signal...\n");
    sleep(3);
    printf("Sighandler handled\n");
}


int main(int argc, char *argv[])
{
    signal(SIGINT, sighandler);
    printf("Hello\n"); 
    while(true) {
        printf("Waiting\n");
        sleep(1);
    }
    return 0;
}

