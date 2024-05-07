#include "../src/core.h"

#define watch(arr, op) \
    printf("[%s] after %s\n", #arr, (op)); \
    printf("capacity: %zu count: %zu data: ", array_capacity(arr), array_count(arr)); \
    for (usize i = 0; i < array_count(arr); ++i) { \
        printf("%d ", (arr)[i]); \
    } \
    printf("\n\n");

int main(void) {
    int *arr = NULL;
    defer {
        array_free(arr);
        watch(arr, "free");
    };

    array_push(arr, 3);
    array_push(arr, 2);
    array_push(arr, 1);

    watch(arr, "push");

    return 0;
}