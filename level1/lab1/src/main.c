#include <stdio.h>
#include <stdlib.h>

unsigned int array_size = 3;

int cmp(const void* a, const void* b) {
    return *((int*)a) - *((int*)b);
}

int main(void) {
    printf("Enter number of elements:\n");
    if (scanf("%u", &array_size) != 1) {
        perror("Invalid input. Expected float values");
        exit(EXIT_FAILURE);
    }

    int arr[3] = {0};
    for (unsigned int i = 0; i < array_size; i++) {
        if (scanf("%d", &arr[i]) != 1) {
            perror("Invalid input. Expected float values");
            exit(EXIT_FAILURE);
        }
    }

    qsort(arr, array_size, sizeof(int), cmp); 

    for (unsigned int i = 0; i < array_size; i++) {
        printf("%d ", arr[i]);
    }
    
    return EXIT_SUCCESS;
}

