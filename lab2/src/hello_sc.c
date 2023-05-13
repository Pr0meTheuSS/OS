// Copyright 2023 Olimpiev Y.

#include <assert.h>
#include <unistd.h>
//#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h> 

void print_hello(void) {
    __asm(
    
            );
    write(1, "hello friend", 12);
}

int main(void) {
    print_hello();
    return EXIT_SUCCESS;
}

