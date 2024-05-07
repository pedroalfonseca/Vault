#include "../src/core.h"

#include <stdio.h>

int main(void) {
    ubyte buffer[KB(1)];
    arena a;
    arena_init(&a, buffer, 256);

    int *x;
    float *f;
    char *str;

    puts("-- arena test --");
    for (int i = 0; i < 10; ++i) {
        // Reset all arena offsets for each loop
        arena_clear(&a);

        x = (int *)arena_alloc(&a, sizeof(int));
        f = (float *)arena_alloc(&a, sizeof(float));
        str = arena_alloc(&a, 10);

        *x = 123;
        *f = 987.0f;
        memmove(str, "Hellope", 7);

        printf("%p: %d\n", x, *x);
        printf("%p: %f\n", f, *f);
        printf("%p: %s\n", str, str);

        str = arena_resize(&a, str, 10, 16);
        memmove(str + 7, " world!", 7);
        printf("%p: %s\n", str, str);
    }

    puts("-- pool test --");
    pool p;
    pool_init(&p, buffer, KB(1), 64);

    void *v0, *v1, *v2, *v3, *v4, *v5;

    v0 = pool_alloc(&p);
    v1 = pool_alloc(&p);
    v2 = pool_alloc(&p);
    v3 = pool_alloc(&p);
    v4 = pool_alloc(&p);
    v5 = pool_alloc(&p);

    printf("After Alloc:\n");

    printf("v0 = %p\n", v0);
    printf("v1 = %p\n", v1);
    printf("v2 = %p\n", v2);
    printf("v3 = %p\n", v3);
    printf("v4 = %p\n", v4);
    printf("v5 = %p\n", v5);

    printf("v1 - v0 = %zu\n", v1 - v0);
    printf("v2 - v1 = %zu\n", v2 - v1);
    printf("v3 - v2 = %zu\n", v3 - v2);
    printf("v4 - v3 = %zu\n", v4 - v3);
    printf("v5 - v4 = %zu\n", v5 - v4);

    pool_free(&p, v5);
    pool_free(&p, v2);
    pool_free(&p, v1);
    pool_free(&p, v3);

    printf("After Freeing v5, v2, v1 and v3:\n");

    printf("v0 = %p\n", v0);
    printf("v1 = %p\n", v1);
    printf("v2 = %p\n", v2);
    printf("v3 = %p\n", v3);
    printf("v4 = %p\n", v4);
    printf("v5 = %p\n", v5);

    printf("v1 - v0 = %zu\n", v1 - v0);
    printf("v2 - v1 = %zu\n", v2 - v1);
    printf("v3 - v2 = %zu\n", v3 - v2);
    printf("v4 - v3 = %zu\n", v4 - v3);
    printf("v5 - v4 = %zu\n", v5 - v4);

    v3 = pool_alloc(&p);

    printf("After Allocating v3:\n");

    printf("v3 = %p\n", v3);

    pool_free(&p, v0);

    printf("After Freeing v0:\n");

    printf("v0 = %p\n", v0);
    printf("v1 = %p\n", v1);
    printf("v2 = %p\n", v2);
    printf("v3 = %p\n", v3);
    printf("v4 = %p\n", v4);
    printf("v5 = %p\n", v5);

    printf("v1 - v0 = %zu\n", v1 - v0);
    printf("v2 - v1 = %zu\n", v2 - v1);
    printf("v3 - v2 = %zu\n", v3 - v2);
    printf("v4 - v3 = %zu\n", v4 - v3);
    printf("v5 - v4 = %zu\n", v5 - v4);

    printf("After Allocating v0:\n");

    printf("v0 = %p\n", v0);

    v0 = pool_alloc(&p);

    pool_free(&p, v4);
    pool_free(&p, v0);
    pool_free(&p, v3);

    return 0;
}