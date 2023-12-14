#define _GNU_SOURCE

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

pthread_t tids[2];

void* worker(void* arg) {
    printf("mythread [%d %d %d]: Hello from mythread!\n", getpid(), getppid(), gettid());
    sleep(5);
    printf("Finish my work...");
}

void* waiter(void* arg) {
    printf("Waiter thread init\n");
    int err = pthread_join(tids[0], NULL);
    printf("Error in waiter thread: %d\n", err);
}


int main(int argc, char** argv) {	
	int ret = 0;
	printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
    int err = pthread_create(&tids[0], NULL, worker, NULL);
    if (err) {
        printf("main: pthread_create() failed: %s\n", strerror(err));
        exit(-1);
    }

    err = pthread_create(&tids[1], NULL, waiter, &tids[0]);
    if (err) {
        printf("main: pthread_create() failed: %s\n", strerror(err));
        exit(-1);
    }

    sleep(3);
    printf("Main before waiting\n");
    for (int i = 0; i < 2; i++) {
        int err = pthread_join(tids[i], NULL);
        printf("Wait result into the main for thread num: %d\n", i);
        printf("main: pthread_join() failed: %s\n", strerror(err));
    }

	exit(-1);
}
