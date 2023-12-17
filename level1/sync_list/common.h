#pragma once
#include <stdlib.h>   // for size_t.

// Генерация случайной строки заданной длины
void generateRandomString(char* str, size_t length);

// Генерация случайной строки случайной длины
void generateRandomLimitedString(char* dest, size_t max_len);

int less(size_t a, size_t b);

int great(size_t a, size_t b);

int equal(size_t a, size_t b);
