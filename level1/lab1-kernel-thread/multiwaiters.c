#define _GNU_SOURCE

#include <stdio.h>
#include "src/thread.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

uintptr_t tids[2];

void* worker(void* arg) {
    sleep(5);
}

void* waiter(void* arg) {
    printf("Waiter thread init\n");
    int err = join_thread(tids[0], NULL);
    printf("Error in waiter thread: %d\n", err);
}

int main(int argc, char** argv) {	
	int ret = 0;
	printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
    int err = create_thread(&tids[0], worker, NULL);
    if (err) {
        printf("main: pthread_create() failed: %s\n", strerror(err));
        exit(-1);
    }

    err = create_thread(&tids[1], waiter, &tids[0]);
    if (err) {
        printf("main: pthread_create() status: %s\n", strerror(err));
        exit(-1);
    }
    
    sleep(3);

    for (int i = 0; i < 2; i++) {
        int err = join_thread(tids[i], NULL);
        printf("Wait result into the main for thread num: %d\n", i);
        if (err) {
            printf("main: pthread_create() status: %s\n", strerror(err));
            exit(-1);
        }
    }

	exit(0);
}
