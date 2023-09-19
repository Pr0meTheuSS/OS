// Copyright 2023 Olimpiev Y.

#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void custom_syscall() {
    const char message[] = "Hello, world!\n";
    unsigned long len = sizeof(message) - 1;

    asm volatile (
        "mov $1000, %%rax\n"
        "mov $1000, %%rdi\n"
        "mov %0, %%rsi\n"
        "mov %1, %%rdx\n"
        "syscall"
        :
        : "r" (message), "r" (len)
        : "%rax", "%rdi", "%rsi", "%rdx"
    );
}

void print_hello(void) {
    custom_syscall();
}

int main(void) {
    print_hello();
    return EXIT_SUCCESS;
}

