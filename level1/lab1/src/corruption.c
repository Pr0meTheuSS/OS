#define _GNU_SOURCE
#define CITY

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

int* var_addrr = NULL;

void* mythread(void* arg) {
    int var = 10;
    var_addrr = &var;
    sleep(2);
    printf("My local var: %d\n", var);
}

void* changer(void* arg) {
    *var_addrr = 100500;
}


int main(int argc, char** argv) {
	
    pthread_t tids[2];
	int err;
    
	printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
    
		err = pthread_create(&tids[0], NULL, mythread, NULL);
		if (err) {
			printf("main: pthread_create() failed: %s\n", strerror(err));
			return -1;
		}		
		err = pthread_create(&tids[1], NULL, changer, NULL);
		if (err) {
			printf("main: pthread_create() failed: %s\n", strerror(err));
			return -1;
		}		


        if (err = pthread_join(tids[1], NULL)) {
            printf("main: pthread_join() failed: %s\n", strerror(err));
            return -1;
        }
        if (err = pthread_join(tids[0], NULL)) {
            printf("main: pthread_join() failed: %s\n", strerror(err));
            return -1;
        }

	return 0;
}

