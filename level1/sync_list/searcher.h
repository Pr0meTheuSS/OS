#define _GNU_SOURCE
#pragma once

#include "list.h"

typedef int (*comparator)(size_t, size_t);
typedef void (*count_fitable_pairs_func)(List*, comparator);

typedef struct Searcher
{
    size_t id;
    count_fitable_pairs_func count_func;
    comparator comparator;
} Searcher;

typedef struct SearchThreadParams {
    Searcher searcher;
    List* list;
} SearchThreadParams;

void base_count_fitable_pairs_func_impl(List* list, int (*comarator)(size_t, size_t));

void* search_thread(void *arg);
