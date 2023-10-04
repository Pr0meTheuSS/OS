#define _GNU_SOURCE
#define CITY

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

int threads_amount = 10;

void* mythread(void* arg) {
	    printf("mythread [%d %d %d]: Hello from mythread!\n", getpid(), getppid(), gettid());
	sleep(15);
}

int main(int argc, char** argv) {
    if (argc == 2) {
        threads_amount = atoi(argv[1]);
    }
	
    pthread_t* tids = (pthread_t*)calloc(threads_amount, sizeof(pthread_t));
	int err;

	printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
	while(1) {
		pthread_t tid;
		err = pthread_create(&tids[i], NULL, (void*)&threads_amount, NULL);
		if (err) {
			printf("%d\n", err);
			printf("main: pthread_create() failed: %s\n", strerror(err));
			printf("%ld", i);
			return -1;
		}		
	}

	// int* ret = (int*)malloc(sizeof(int));

	for (size_t i = 0; i < threads_amount; i++) {
		#ifdef WILLAGE
			// Willage-style
			sleep(1);
		#endif

		#ifdef CITY
			// City-style
			if (err = pthread_join(tids[i], NULL)) {
				printf("main: pthread_join() failed: %s\n", strerror(err));
				return -1;
			}
		#endif
			// Используем результат
			// printf("Result from thread[%ld]: %d\n", tids[i], *ret);
	}
	
	free(tids);
	return 0;
}

