// #define _GNU_SOURCE
// #define CITY

// #include <stdio.h>
// #include <pthread.h>
// #include <string.h>
// #include <stdlib.h>
// #include <errno.h>
// #include <sys/types.h>
// #include <unistd.h>


// void* mythread(void* arg) {
//     printf("mythread: I am there\n");
//     printf("mythread: I am there\n");
//     printf("mythread: I am there\n");
//     printf("mythread: I am there\n");
//     printf("mythread: I am there\n");
// }

// int main(int argc, char** argv) {	
//     pthread_t tids[2];
// 	int err;
    
// 	printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
    
// 		err = pthread_create(&tids[0], NULL, mythread, NULL);
// 		if (err) {
// 			printf("main: pthread_create() failed: %s\n", strerror(err));
// 			return -1;
// 		}		
//         pthread_cancel(tids[0]);

//         if (err = pthread_join(tids[0], NULL)) {
//             printf("main: pthread_join() failed: %s\n", strerror(err));
//             return -1;
//         }

// 	return 0;
// }

// #define _GNU_SOURCE
// #define CITY

// #include <stdio.h>
// #include <pthread.h>
// #include <string.h>
// #include <stdlib.h>
// #include <errno.h>
// #include <sys/types.h>
// #include <unistd.h>

// int global_var = 0;

// void* mythread(void* arg) {
//     while(1) {
//         global_var++;
//     }
//     printf("mythread: I am there\n");
// }

// int main(int argc, char** argv) {	
//     pthread_t tids[2];
// 	int err;
    
// 	printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
    
// 		err = pthread_create(&tids[0], NULL, mythread, NULL);
// 		if (err) {
// 			printf("main: pthread_create() failed: %s\n", strerror(err));
// 			return -1;
// 		}		
//         pthread_cancel(tids[0]);

//         if (err = pthread_join(tids[0], NULL)) {
//             printf("main: pthread_join() failed: %s\n", strerror(err));
//             return -1;
//         }

// 	return 0;
// }


// #define _GNU_SOURCE
// #define CITY

// #include <stdio.h>
// #include <pthread.h>
// #include <string.h>
// #include <stdlib.h>
// #include <errno.h>
// #include <sys/types.h>
// #include <unistd.h>

// int global_var = 0;

// void* mythread(void* arg) {
//     while(1) {
//         global_var++;
//         pthread_testcancel();
//     }
//     printf("mythread: I am there\n");
// }

// int main(int argc, char** argv) {	
//     pthread_t tids[2];
// 	int err;
    
// 	printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
    
// 		err = pthread_create(&tids[0], NULL, mythread, NULL);
// 		if (err) {
// 			printf("main: pthread_create() failed: %s\n", strerror(err));
// 			return -1;
// 		}		
//         pthread_cancel(tids[0]);

//         if (err = pthread_join(tids[0], NULL)) {
//             printf("main: pthread_join() failed: %s\n", strerror(err));
//             return -1;
//         }

// 	return 0;
// }

// #define _GNU_SOURCE
// #define CITY

// #include <stdio.h>
// #include <pthread.h>
// #include <string.h>
// #include <stdlib.h>
// #include <errno.h>
// #include <sys/types.h>
// #include <unistd.h>

// int global_var = 0;

// void* mythread(void* arg) {
//     int oldtype;
//     pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);
//     while(1) {
//         global_var++;
//     }
//     printf("mythread: I am there\n");
// }

// int main(int argc, char** argv) {	
//     pthread_t tids[2];
// 	int err;
    
// 	printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
    
// 		err = pthread_create(&tids[0], NULL, mythread, NULL);
// 		if (err) {
// 			printf("main: pthread_create() failed: %s\n", strerror(err));
// 			return -1;
// 		}		
//         pthread_cancel(tids[0]);

//         if (err = pthread_join(tids[0], NULL)) {
//             printf("main: pthread_join() failed: %s\n", strerror(err));
//             return -1;
//         }

// 	return 0;
// }


#define _GNU_SOURCE
#define CITY

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

int global_var = 0;
pthread_t tids[2];

void* mythread(void* arg) {
    while(1) {
        global_var++;
        pthread_cancel(tids[0]);
    }
    printf("mythread: I am there\n");
}


int main(int argc, char** argv) {	
	int err;
    
	printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
    
		err = pthread_create(&tids[0], NULL, mythread, NULL);
		if (err) {
			printf("main: pthread_create() failed: %s\n", strerror(err));
			return -1;
		}		

        if (err = pthread_join(tids[0], NULL)) {
            printf("main: pthread_join() failed: %s\n", strerror(err));
            return -1;
        }

	return 0;
}
