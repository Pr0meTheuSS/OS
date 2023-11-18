#include "thread.h"

int start_thread(void* th) {
    assert(th);
    thread* t = (thread*)th;
    t->finished = 0;
    t->joined = 0;

    t->retval = t->routine(t->args);

    t->finished = 1;
    wake_futex_blocking(&(t->finished));
    return 0;
}

thread* init_thread_struct(uintptr_t* tid, start_routine func, void* args) {
    thread* th = (thread*)malloc(sizeof(thread));
    if (!th) {
        return NULL;
    }

    *tid = th->tid = (uintptr_t) th;

    th->st = create_stack(8 * getpagesize());
    if (!th->st) {
        free(th);
        return NULL;
    }

    th->args = args;
    th->routine = func;
    th->joined = 0;
    th->finished = 0;

    return th;
}

int create_thread(uintptr_t* tid, start_routine func, void* args) {
    thread* th = init_thread_struct(tid, func, args);
    if (!th) {
        return -1;
    }

    const int clone_flags = (
                CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SYSVSEM
                | CLONE_SIGHAND | CLONE_THREAD
                | 0);

    int ret = clone(start_thread,
                    (unsigned char*)th->st->stack_ptr + th->st->stack_size,
                    clone_flags,
                    (void*)th);

    if (errno != 0) {
        perror(strerror(errno));
        goto failed;
    }
    return ret;
failed:
    destroy_stack(th->st);
    free(th);
    return -1;
}

int join_thread(uintptr_t tid, void** ret) {
    const int zero = 0;
    thread* curr_th = (thread*) tid;
    wait_on_futex_value(&(curr_th->finished), 1);
    atomic_compare_exchange_strong(&(curr_th->joined), &zero, 1);

    if (ret) {
      *ret = curr_th->retval;
    }

    destroy_thread(curr_th);

    return 0;
}

void destroy_thread(thread* th) {
    destroy_stack(th->st);
    free(th);
}
