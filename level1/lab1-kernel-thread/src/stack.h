#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/mman.h>
#include <signal.h>

typedef struct {
    void*   stack_ptr;
    size_t  stack_size;
    void*   guard_page;
} Stack;

Stack* create_stack(size_t size);
void   destroy_stack(Stack *stack);