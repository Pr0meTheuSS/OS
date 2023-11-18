#define _GNU_SOURCE
#include <assert.h>
#include <err.h>
#include <errno.h>
#include <sched.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "./thread.h"

void* func(void* args) {
    if (!args) {
      sleep(5);
      printf("\nChild: [pid:%d ppid:%d tid:%d]\n", getpid(), getppid(), gettid());
    }

    return NULL;
}

int main(void) {
    printf("Main: [pid:%d ppid:%d tid:%d]\n", getpid(), getppid(), gettid());
    uintptr_t tid;
    printf("Res: %d\n", create_thread(&tid, func, NULL));

    join_thread(tid, NULL);
    return 0;
}
