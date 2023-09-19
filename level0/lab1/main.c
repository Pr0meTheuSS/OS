// Copyright 2023 Olimpiev Y.
#include <stdlib.h>

#include "static/hello_static_lib.h"

int main(void) {
   // call static lib's hello function.
   hello_from_static_lib();
   return EXIT_SUCCESS;
}
