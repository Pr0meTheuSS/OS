#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "searcher.h"

void base_count_fitable_pairs_func_impl(List* list, int (*comparator)(size_t, size_t)) {
    assert(list);
    assert(list->head);

    ListNode* current = list->head;
    for (; current && current->next; current = current->next) {
        pthread_mutex_lock(&current->sync);
        size_t len1 = strlen(current->value);

        for (ListNode* other = current->next; other; other = other->next) {
            pthread_mutex_lock(&current->next->sync);
            size_t len2 = strlen(current->next->value);
            if (comparator(len1, len2)) {
                // todo: something...
            }
            pthread_mutex_unlock(&current->next->sync);
        }

        pthread_mutex_unlock(&current->sync);
    }
}

void* search_thread(void *arg) {
    printf("Init thread for searching...\n");
    SearchThreadParams* params = (SearchThreadParams*)arg;

    for (;;) {
        params->searcher.count_func(params->list, params->searcher.comparator);
        // printf("Thread %zu with id %zu\n", pthread_self(),  params->searcher.id);
        params->list->threads_attempts_amount[params->searcher.id]++;
    }

    return NULL;
}
