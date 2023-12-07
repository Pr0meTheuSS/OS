#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

void print_ids(const char* header) {
	printf("%s. [%d %d %d]\n", header, getpid(), getppid(), gettid());
}

void* mythread(void* arg) {
    print_ids("Hello from mythread");
    return NULL;
}

int main() {
	pthread_t tid;
	int err;
    
    print_ids("Hello from main");

	err = pthread_create(&tid, NULL, mythread, NULL); 
    if (err) {
	    printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}

	return 0;
}

