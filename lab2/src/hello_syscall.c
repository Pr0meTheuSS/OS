// Copyright 2023 Olimpiev Y.
// Build: gcc hello.c -o hello

#include <assert.h>
#include <stdio.h>
#include <stdlib.h> 
#include <sys/syscall.h>
#include <unistd.h>

void print_hello(void) {
    syscall(1, 1, "hello friend", 12);
}

int main(void) {
    print_hello();
    
    return EXIT_SUCCESS;
}
