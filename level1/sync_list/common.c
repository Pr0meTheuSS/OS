#include <assert.h>
#include "common.h"

// Генерация случайной строки заданной длины
void generateRandomString(char* str, size_t length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < length - 1; ++i) {
        int index = rand() % (sizeof(charset) - 1);
        str[i] = charset[index];
    }
    
    str[length - 1] = '\0';  // Убедимся, что строка завершается нулевым символом
}


// Генерация случайной строки случайной длины
void generateRandomLimitedString(char* dest, size_t max_len) {
    assert(dest);
    size_t len = rand() % (max_len - 1) + 1;
    generateRandomString(dest, len);
}

int less(size_t a, size_t b) {
    return a < b;
}

int great(size_t a, size_t b) {
    return a > b;
}

int equal(size_t a, size_t b) {
    return a == b;
}
