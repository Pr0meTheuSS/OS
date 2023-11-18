#pragma once
#define _GNU_SOURCE
#include <assert.h>
#include <err.h>
#include <errno.h>
#include <sched.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <unistd.h>
#include "stack.h"
#include <linux/futex.h>

#include <stdatomic.h>
#include <sys/syscall.h>
#include <linux/futex.h>

#include "stack.h"
#include "futex.h"

typedef void* (*start_routine)(void *);

typedef struct thread {
    uintptr_t           tid;
    start_routine       routine;
    Stack*              st;
    void* restrict      args;
    void* restrict      retval;
    int                 joined;
    int                 finished;
} thread;

int start_thread(void* th);
thread* init_thread_struct(uintptr_t* tid, start_routine func, void* args);
int create_thread(uintptr_t* tid, start_routine func, void* args);
int join_thread(uintptr_t tid, void** ret);
void destroy_thread(thread* th);