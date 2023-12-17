#include <pthread.h>

#include "common.h"
#include "list.h"
#include "lmonitor.h"
#include "searcher.h"
#include "swapper.h"

int main() {
    List my_list = { .head = NULL, .successfully_swaped_amount = 0, .threads_attempts_amount = {0, 0, 0}};
    initialize_list_with_random_strings(&my_list, 10);

    pthread_t swapper_tids[3];
    pthread_t searchers_tids[3];
    comparator comps[3] = {less, equal, great};

    SearchThreadParams params[3] = 
    {
        {.list = &my_list, .searcher = { .id = 0, .count_func = base_count_fitable_pairs_func_impl, .comparator = comps[0]} },
        {.list = &my_list, .searcher = { .id = 1, .count_func = base_count_fitable_pairs_func_impl, .comparator = comps[1]} },
        {.list = &my_list, .searcher = { .id = 2, .count_func = base_count_fitable_pairs_func_impl, .comparator = comps[2]} }
    };

    // Запускаем потоки с поиском.
    for (size_t i = 0; i < sizeof(searchers_tids) / sizeof(searchers_tids[0]); i++) {
        pthread_create(&searchers_tids[i], NULL, search_thread, &params[i]);
    }

    // Запускаем потоки с обменом.
    for (size_t i = 0; i < sizeof(swapper_tids) / sizeof(swapper_tids[0]); i++) {
        pthread_create(&swapper_tids[i], NULL, swap_thread_func, (void*)&my_list);
    }


    // Запускаем монитор.
    pthread_t monitor_tid[1];
    pthread_create(&monitor_tid[0], NULL, print_stats, (void*)&my_list);


    for (size_t i = 0; i < sizeof(searchers_tids) / sizeof(searchers_tids[0]); i++) {
        pthread_join(searchers_tids[i], NULL);
    }

    for (size_t i = 0; i < sizeof(swapper_tids) / sizeof(swapper_tids[0]); i++) {
        pthread_join(swapper_tids[i], NULL);
    }

    pthread_join(monitor_tid[0], NULL);

    freeList(&my_list);
    return 0;
}
