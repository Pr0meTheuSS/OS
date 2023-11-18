#pragma once
#define _GNU_SOURCE
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <linux/futex.h>
#include <stdatomic.h>
#include <sys/syscall.h>
// #include <unistd.h>

#include "stack.h"

void wait_on_futex_value(int* futex_addr, int val);

void wake_futex_blocking(int* futex_addr);
