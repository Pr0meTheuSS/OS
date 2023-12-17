#pragma once
#include <pthread.h>

#define MAX_STRING_LENGTH 100

typedef struct ListNode {
    char value[MAX_STRING_LENGTH];
    struct ListNode* next;
    pthread_mutex_t sync;
} ListNode;

typedef struct List {
    ListNode* head;
    _Atomic size_t threads_attempts_amount[3];
    _Atomic size_t successfully_swaped_amount;
} List;

void initializeListWithRandomStrings(List* list, int numElements);

void append(List* list, const char* value);

void freeList(List* list);

