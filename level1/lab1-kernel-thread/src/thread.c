#include "thread.h"

int start_thread(void* th) {
    assert(th);

    thread* t = (thread*)th;
    t->finished = 0;
    t->joined = 0;

    t->retval = t->routine(t->args);

    wake_futex_blocking(&(t->finished), 1);
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
    th->waiter = 0;

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

    clone(start_thread,
            (unsigned char*)th->st->stack_ptr + th->st->stack_size,
            clone_flags,
            (void*)th);

    if (errno != 0) {
        perror(strerror(errno));
        goto failed;
    }
    return 0;
failed:
    destroy_stack(th->st);
    free(th);
    return -1;
}

int join_thread(uintptr_t tid, void** ret) {
    const int one = 1;
    const int zero = 0;

    thread* curr_th = (thread*) tid;
    if (atomic_compare_exchange_strong(&(curr_th->joined), &one, 1)) {
        // Already joined.
        printf("Already joined\n");
        return 0;
    }

    if (!atomic_compare_exchange_strong(&(curr_th->waiter), &zero, 1)) {
        // Already waiting.
        printf("Somebody waited, sorry\n");
        return -1;
    }


    wait_on_futex_value(&(curr_th->finished), 1);

    if (ret) {
      *ret = curr_th->retval;
    }

    destroy_thread(curr_th);
    // curr_th = NULL;
    return 0;
}

void destroy_thread(thread* th) {
    destroy_stack(th->st);
    free(th);
}
