//#define MEMDBG_PRINT_ALL
//#include "../src/memdbg.h"
#include "../src/array.h"

#include <stdio.h>

#define watch(arr, op) \
    printf("[%s] after %s\n", #arr, (op)); \
    printf("capacity: %zu count: %zu data: ", array_capacity(arr), array_count(arr)); \
    for (usize i = 0; i < array_count(arr); ++i) { \
        printf("%d ", (arr)[i]); \
    } \
    printf("\n\n");

int int_cmp(const void *a, const void *b) {
    if (*(int *)a < *(int *)b) {
        return -1;
    }

    if (*(int *)a > *(int *)b) {
        return +1;
    }

    return 0;
}

int main(void) {
    puts("-- array test --");

    int *arr1 = NULL;

    watch(arr1, "creation");

    array_push(arr1, -1);
    array_push(arr1, 2);

    watch(arr1, "push");

    array_insert(arr1, 1, 0);
    array_insert(arr1, 2, 1);

    watch(arr1, "insert");

    array_reserve(arr1, 9);

    watch(arr1, "reserve");

    int elems[] = {3, 4, 5, 6, 7, 8};
    array_concat(arr1, elems, countof(elems));

    watch(arr1, "concat");

    int *arr2 = NULL;
    array_copy(arr2, arr1);

    watch(arr2, "copy");

    array_free(arr2);

    //memdbg_print_stats(stderr, true);

    array_pop(arr1);

    watch(arr1, "pop");

    array_remove(arr1, 0);

    watch(arr1, "remove");

    array_clear(arr1);

    watch(arr1, "clear");

    array_push(arr1, 3);
    array_push(arr1, 2);
    array_push(arr1, 1);
    array_sort(arr1, int_cmp);

    watch(arr1, "sort");

    array_free(arr1);

    watch(arr1, "free");

    //memdbg_print_stats(stderr, true);

    return 0;
}