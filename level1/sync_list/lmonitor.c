#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "list.h"

void* print_stats(void* arg) {
    List* list = (List*)arg;
    for (;;) {
        system("clear");
        printf("List Stats\n-------------------------------\n");
        printf("Successfully swaps: %zu\n", list->successfully_swaped_amount);

        for (size_t i = 0; i < sizeof(list->threads_attempts_amount) / sizeof(list->threads_attempts_amount[0]); i++) {
            printf("Thread [%zu]: iterations: %zu\n", i, list->threads_attempts_amount[i]);
        }

        sleep(2);
    }

    return NULL;
}