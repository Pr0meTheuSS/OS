#include <assert.h>
#include <string.h>

#include "common.h"
#include "list.h"

// Инициализация списка с заданным числом элементов
void initialize_list_with_random_strings(List* list, int numElements) {
    assert(list);

    for (int i = 0; i < numElements; ++i) {
        char randomString[MAX_STRING_LENGTH];
        generate_random_limited_string(randomString, MAX_STRING_LENGTH);
        append(list, randomString);
    }
}

void append(List* list, const char* value) {
    assert(list);
    assert(value);

    ListNode* new_node = (ListNode*)malloc(sizeof(ListNode));
    assert(new_node);

    strncpy(new_node->value, value, strlen(value));
    new_node->value[strlen(value)] = '\0';

    new_node->next = NULL;
    pthread_mutex_init(&new_node->sync, NULL);

    if (!list->head) {
        list->head = new_node;
    } else {
        ListNode* current = list->head;
        while (current->next) {
            current = current->next;
        }
        current->next = new_node;
    }
}

// Функция для удаления списка (освобождения памяти)
void freeList(List* list) {
    assert(list);

    ListNode* current = list->head;
    ListNode* next;

    while (current) {
        next = current->next;
        pthread_mutex_destroy(&current->sync);
        free(current);
        current = next;
    }

    list->head = NULL;
}
