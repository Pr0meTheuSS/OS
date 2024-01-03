#include "stack.h"

Stack* create_stack(size_t size) {
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    if (stack == NULL) {
        perror("malloc");
        goto failed;
    }

    // Округляем размер стека до ближайшей страницы
    size_t page_size = getpagesize();
    size_t rounded_size = (size + page_size - 1) & ~(page_size - 1);

    // Выделяем память для стека с правами MAP_PRIVATE
    stack->stack_ptr = mmap(NULL, rounded_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
    if (stack->stack_ptr == MAP_FAILED) {
        perror("mmap");
        goto failed;
    }

    stack->stack_size = rounded_size;

    // Выделяем память для защитной страницы
    stack->guard_page = mmap(NULL, getpagesize(), PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (stack->guard_page == MAP_FAILED) {
        perror("mmap");
        goto failed;
    }

    if (mprotect(stack->guard_page, page_size, PROT_NONE) != 0) {
        perror("mprotect");
        goto failed;
    }

    return stack;
failed:
    destroy_stack(stack);
    return NULL;
}

void destroy_stack(Stack *stack) {
    if (stack != NULL) {
        if (stack->stack_ptr != NULL) {
            munmap(stack->stack_ptr, stack->stack_size);
        }
        // if (stack->guard_page != NULL) {
        //     munmap(stack->guard_page, getpagesize());
        // }

        //free(stack);
    }
}
