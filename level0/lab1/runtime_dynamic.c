// Copyright 2023 Olimpiev Y.
#include <dlfcn.h>
#include <stdio.h>

int main(void) {
	void* extlib;
	void (*hellofuncptr)(void);

	extlib = dlopen("/home/prometheus/Рабочий стол/OS/lab1/libdhello.so", RTLD_LAZY);
	if (!extlib) {
		fprintf(stderr, "dlopen() error: %s\n", dlerror());
		return 1;
	}

	hellofuncptr = dlsym(extlib, "hello_from_dynamic_lib");
    if (!hellofuncptr) {
        fprintf(stderr, "dlsym() error: %s\n", dlerror());
   	    dlclose(extlib);
        return 1;
    }

    hellofuncptr();

	dlclose(extlib);

	return 0;
}

