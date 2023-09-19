#define _GNU_SOURCE
#define CITY
#define THREADS_AMOUNT 1

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

void* mythread(void* arg) {
	printf("mythread [%d %d %d]: Hello from mythread!\n", getpid(), getppid(), gettid());
	return NULL;
}

int main() {
	pthread_t tid[THREADS_AMOUNT];
	int err;

	printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

	for (size_t i = 0; i < THREADS_AMOUNT; i++) {
		err = pthread_create(&tid[i], NULL, mythread, NULL);
		if (err) {
			printf("main: pthread_create() failed: %s\n", strerror(err));
			return -1;
		}		
	}


	for (size_t i = 0; i < THREADS_AMOUNT; i++) {
		#ifdef WILLAGE
			// Willage-style
			sleep(10);
		#endif

		#ifdef CITY
			// City-style
			void* thrd_ret[THREADS_AMOUNT];
			if (err = pthread_join(tid[i], &thrd_ret[i])) {
				printf("main: pthread_join() failed: %s\n", strerror(err));
				return -1;
			}
		#endif
	}

	return 0;
}
