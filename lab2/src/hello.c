// Copyright 2023 Olimpiev Y.
// Build: gcc hello.c -o hello

#include <assert.h>
#include <stdio.h>
#include <stdlib.h> 
//#define CHECK_RUN

void print_hello(void) {
    printf("Hello world\n");
}

int main(void) {
    print_hello();
    
    #ifdef CHECK_RUN
        FILE* f = fopen("test.dat", "wb");
        assert(f);
        fprintf(f, "hello world in file.\n");
        fclose(f);
    #endif
    
    return EXIT_SUCCESS;
}
