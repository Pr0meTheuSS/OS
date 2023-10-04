#define _GNU_SOURCE

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

int threads_amount = 10;

void* mythread(void* arg) {
    while (1) {
        printf("mythread [%d %d %d]: Hello from mythread!\n", getpid(), getppid(), gettid());
    }
}

int main(int argc, char** argv) {	
	int ret = 0;
	printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
    int i = 0;
	while(1) {
        i++;
		pthread_t tid;
		int err = pthread_create(&tid, NULL, mythread, NULL);
		if (err) {
			// printf(errno);
			// printf("%d\n\b\r", err);
			// printf("%d\n\r", err);
			printf("main: pthread_create() failed: %s %d\n", strerror(err), i);
			printf("%d\n\r", i);
			exit(-1);
			// pthread_exit(&ret);
		}
	}

			exit(-1);
	// pthread_exit(&ret);
}
