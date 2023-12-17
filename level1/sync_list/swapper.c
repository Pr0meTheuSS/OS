#include <assert.h>
#include <stdlib.h>
#include "list.h"

void random_nodes_swap(List* list) {
    assert(list);
    assert(list->head);

    pthread_mutex_lock(&list->head->sync);
    ListNode* nodes[3];
    
    for (nodes[0] = list->head; nodes[0]->next != NULL; nodes[0] = nodes[1]) {
        pthread_mutex_lock(&nodes[0]->next->sync);
        nodes[1] = nodes[0]->next;
        if (nodes[1]->next != NULL) {
            pthread_mutex_lock(&nodes[1]->next->sync);
            nodes[2] = nodes[1]->next;
            if (nodes[2] && rand() % 2) {
                list->successfully_swaped_amount++;
                nodes[0]->next = nodes[2];
                nodes[1]->next = nodes[2]->next;
                nodes[2]->next = nodes[1];
            }
            pthread_mutex_unlock(&nodes[2]->sync);
        }
        pthread_mutex_unlock(&nodes[0]->sync);
    }
        
    pthread_mutex_unlock(&nodes[0]->sync);
}

static int iteration = 0;
void* swap_thread_func(void* arg) {
    List* list = (List*)arg;
    for (;;) {
        random_nodes_swap(list);
    }
}
