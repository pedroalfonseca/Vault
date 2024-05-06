#ifndef ARRAY_H
#define ARRAY_H

#include "types.h"

#include <stdlib.h>
#include <string.h>

// --------------------------------------------------------------------------------

#define countof(a)             (sizeof((a)) / sizeof(*(a)))

// --------------------------------------------------------------------------------

// Adapted from https://github.com/nothings/stb/blob/master/stb_ds.h

#define array_capacity(a)      ((a) ? _array_capacity(a) : 0)
#define array_count(a)         ((a) ? _array_count(a) : 0)
#define array_reserve(a, n)    (_array_grow(a, 0, n))
#define array_last(a)          ((a)[_array_count(a) - 1])
#define array_push(a, e)       (_array_mgrow(a, 1) ? (a)[_array_count(a)++] = (e), (a) : NULL)
#define array_pop(a)           (array_count(a) ? --_array_count(a), (a) : NULL)
#define array_insert(a, i, e)  (_array_mgrow(a, 1) ? _array_insert(a, i, e), ++_array_count(a), (a) : NULL)
#define array_remove(a, i)     (((i) >= 0 && (i) < array_count(a)) ? _array_remove(a, i), --_array_count(a), (a) : NULL)
#define array_concat(a, b, n)  (_array_mgrow(a, n) ? _array_concat(a, b, n), _array_count(a) += (n), (a) : NULL)
#define array_copy(a, b)       (*((void **)&(a)) = _array_memcpy((b), array_capacity(b), array_count(b), sizeof(*(b))))
#define array_sort(a, c)       ((a) ? qsort((a), _array_count(a), sizeof(*(a)), (c)), (a) : NULL)
#define array_clear(a)         ((a) ? _array_count(a) = 0, (a) : NULL)
#define array_free(a)          ((a) ? free(_array_header(a)), (a) = NULL, NULL : NULL)

#define _array_header(a)       ((usize *)(a) - 2)
#define _array_capacity(a)     (_array_header(a)[0])
#define _array_count(a)        (_array_header(a)[1])
#define _array_full(a, n)      (!(a) || _array_count(a) + (n) > _array_capacity(a))
#define _array_grow(a, n, m)   (*((void **)&(a)) = _array_realloc((a), (n), sizeof(*(a)), (m)))
#define _array_mgrow(a, n)     (_array_full(a, n) ? (_array_grow(a, n, 0) != 0) : 1)
#define _array_insert(a, i, e) (memmove((a) + (i) + 1, (a) + (i), (_array_count(a) - i) * sizeof(*(a))), (a)[(i)] = (e))
#define _array_remove(a, i)    (memmove((a) + (i), (a) + (i) + 1, (_array_count(a) - 1 - i) * sizeof(*(a))))
#define _array_concat(a, b, n) (memcpy((a) + _array_count(a), (b), (n) * sizeof(*(b))))

internal void *_array_realloc(void *arr, usize num_elems, usize stride, usize min_cap) {
    usize min_count = array_count(arr) + num_elems;

    if (min_count > min_cap) {
        min_cap = min_count;
    }

    if (min_cap <= array_capacity(arr)) {
        return arr;
    }

    usize dbl_cap = 2 * array_capacity(arr);

    if (min_cap < dbl_cap) {
        min_cap = dbl_cap;
    } else if (min_cap < 2) {
        min_cap = 2;
    }

    usize *data = (usize *)realloc(arr ? _array_header(arr) : NULL, 2 * sizeof(usize) + min_cap * stride);
    if (data == NULL) {
        return NULL;
    }

    data[0] = min_cap;

    return data + 2;
}

internal void *_array_memcpy(void *arr, usize cap, usize count, usize stride) {
    if (arr == NULL) {
        return NULL;
    }

    usize arr_size = cap * stride;

    usize *data = (usize *)malloc(2 * sizeof(usize) + arr_size);
    if (data == NULL) {
        return NULL;
    }

    memcpy(data + 2, arr, arr_size);
    data[0] = cap;
    data[1] = count;

    return data + 2;
}

// --------------------------------------------------------------------------------

#endif // ARRAY_H