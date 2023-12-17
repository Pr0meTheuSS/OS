#pragma once
#include <stdlib.h>   // for size_t.

void generate_random_string(char* str, size_t length);

void generate_random_limited_string(char* dest, size_t max_len);

int less(size_t a, size_t b);

int great(size_t a, size_t b);

int equal(size_t a, size_t b);
