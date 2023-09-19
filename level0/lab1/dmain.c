// Copyright 2023 Olimpiev Y.
// add LD_LIBRARY_PATH="/home/prometheus/Рабоdчий стол/OS/lab1"
#include <stdlib.h>

#include "dynamic/hello_dynamic_lib.h"

int main(void) {
   // call dynamic lib's hello function.
   hello_from_dynamic_lib();
   return EXIT_SUCCESS;
}
