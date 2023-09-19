// Copyright 2023 Olimpiev Y.
// Build: gcc hello.c -o hello

#include <stdio.h>
#include <stdlib.h> 

void print_hello(void) {
    printf("Hello world\n");
}

int main(void) {
    print_hello();
    return EXIT_SUCCESS;
}
