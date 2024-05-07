#ifndef MEMDBG_H
#define MEMDBG_H

#include "types.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Adapted from https://github.com/paguso/cocada/blob/master/libcocada/src/core/memdbg.c

// --------------------------------------------------------------------------------

global const char *prefixes[7] = {"", "kilo", "mega", "giga", "tera", "peta", "exa"};

typedef struct _pretty_size {
    f64         size;
    const char *prefix;
} pretty_size;

internal pretty_size prettify(usize size) {
    f64 total = (f64)size;

    usize i = 0;
    while (i < 6 && size >> 10) {
        size >>= 10;
        ++i;
    }

    pretty_size ret;
    ret.size = total / (f64)(1 << (i * 10));
    ret.prefix = prefixes[i];

    return ret;
}

// --------------------------------------------------------------------------------

internal usize fib_hash(void *addr, usize cap) {
    usize h = (usize)addr;
    h *= 11400714819323198485llu;

    return h % cap;
}

// --------------------------------------------------------------------------------

typedef enum _memchunk_state {
    MEMCHUNK_FREE    = 0x00,
    MEMCHUNK_ACTIVE  = 0x01,
    MEMCHUNK_DELETED = 0x10
} memchunk_state;

// --------------------------------------------------------------------------------

typedef struct _memchunk {
    usize           alloc_no;
    usize           size;
    memchunk_state  state;
    void           *addr;
} memchunk;

// --------------------------------------------------------------------------------

typedef struct _memtable {
    usize     num_total, num_act, num_del;

    usize     cap, size;
    memchunk *data;
} memtable;

#define MEMTABLE_MIN_CAP  128
#define MEMTABLE_GROW_BY  1.62
#define MEMTABLE_MIN_LOAD 0.25
#define MEMTABLE_MAX_LOAD 0.66

void memtable_init(memtable *self) {
    if (self->cap > 0) {
        return;
    }

    self->num_total = self->num_act = self->num_del = 0;
    self->cap = MEMTABLE_MIN_CAP, self->size = 0;
    self->data = (memchunk *)calloc(self->cap, sizeof(memchunk));
}

void memtable_mgrow(memtable *self) {
    f64 load = (f64)(self->num_act + self->num_del) / (f64)(self->cap);
    usize new_cap = self->cap;

    if (load < MEMTABLE_MIN_LOAD) {
        new_cap = self->cap / MEMTABLE_GROW_BY;
        new_cap = (new_cap > MEMTABLE_MIN_CAP) ? new_cap : MEMTABLE_MIN_CAP;
    } else if (load > MEMTABLE_MAX_LOAD) {
        new_cap = self->cap * MEMTABLE_GROW_BY;
    }

    if (new_cap != self->cap) {
        memchunk *new_data = (memchunk *)calloc(new_cap, sizeof(memchunk));

        for (usize i = 0; i < self->cap; ++i) {
            if (self->data[i].state == MEMCHUNK_ACTIVE) {
                usize idx = fib_hash(self->data[i].addr, new_cap);

                for (; new_data[idx].state != MEMCHUNK_FREE; idx = (idx + 1) % new_cap);
                new_data[idx] = self->data[i];
            }
        }

        free(self->data);
        self->data = new_data;
        self->num_del = 0;
        self->cap = new_cap;
    }
}

usize memtable_set(memtable *self, void *addr, usize size) {
    memtable_init(self);

    usize idx = fib_hash(addr, self->cap);
    while (self->data[idx].state != MEMCHUNK_FREE) {
        if (self->data[idx].state == MEMCHUNK_ACTIVE && self->data[idx].addr == addr) {
            self->size += (size - self->data[idx].size);
            self->data[idx].size = size;

            return self->data[idx].alloc_no;
        }

        idx = (idx + 1) % self->cap;
    }

    usize ret = self->num_total;

    self->data[idx].alloc_no = self->num_total;
    ++self->num_total;
    self->data[idx].addr = addr;
    self->data[idx].size = size;
    self->data[idx].state = MEMCHUNK_ACTIVE;
    self->size += size;
    ++self->num_act;

    memtable_mgrow(self);

    return ret;
}

void memtable_unset(memtable *self, void *addr) {
    memtable_init(self);

    usize idx = fib_hash(addr, self->cap);
    while (self->data[idx].state != MEMCHUNK_FREE) {
        if (self->data[idx].state == MEMCHUNK_ACTIVE && self->data[idx].addr == addr) {
            self->data[idx].state = MEMCHUNK_DELETED;
            ++self->num_del;
            --self->num_act;
            self->size -= self->data[idx].size;

            break;
        }

        idx = (idx + 1) % self->cap;
    }

    memtable_mgrow(self);
}

typedef struct _memdbg_query_output {
    bool  active;
    usize size;
} memdbg_query_output;

memdbg_query_output memtable_get(memtable *self, void *addr) {
    memdbg_query_output ret = DEFAULT_VAL;

    usize idx = fib_hash(addr, self->cap);
    while (self->data[idx].state != MEMCHUNK_FREE) {
        if (self->data[idx].state == MEMCHUNK_ACTIVE && self->data[idx].addr == addr) {
            ret.active = true;
            ret.size = self->data[idx].size;

            return ret;
        }

        idx = (idx + 1) % self->cap;
    }

    assert(self->data[idx].state == MEMCHUNK_FREE);

    return ret;
}

// --------------------------------------------------------------------------------

typedef struct _alloc_pair {
    usize alloc_no, idx;
} alloc_pair;

internal i32 alloc_pair_cmp(const void *lhs, const void *rhs) {
    if (((alloc_pair *)lhs)->alloc_no < ((alloc_pair *)rhs)->alloc_no) {
        return -1;
    }

    if (((alloc_pair *)lhs)->alloc_no > ((alloc_pair *)rhs)->alloc_no) {
        return +1;
    }

    return 0;
}

// --------------------------------------------------------------------------------

void memtable_print_stats(FILE *stream, memtable *self, bool print_chunks) {
    fprintf(stream, "\x1b[90m================================================================================\033[0m\n");
    fprintf(stream, "                                \x1b[93mheap memory info\033[0m\n");

    if (print_chunks) {
        fprintf(stream, "\x1b[90m--------------------------------------------------------------------------------\033[0m\n");
        fprintf(stream, "\x1b[97m* chunks in chronological order of allocation\033[0m\n\n");

        usize n = self->num_act, k = 0;
        alloc_pair *pairs = (alloc_pair *)malloc(n * sizeof(alloc_pair));

        for (usize i = 0; i < self->cap; ++i) {
            if (self->data[i].state == MEMCHUNK_ACTIVE) {
                pairs[k].alloc_no = self->data[i].alloc_no;
                pairs[k].idx = i;
                ++k;
            }
        }

        assert(k == n);

        qsort(pairs, n, sizeof(alloc_pair), alloc_pair_cmp);

        usize idx;
        for (usize i = 0; i < n; ++i) {
            idx = pairs[i].idx;

            fprintf(stream, "#%zu: %zu bytes at %p\n",
                    pairs[i].alloc_no, self->data[idx].size, self->data[idx].addr);
        }

        fprintf(stream, "\x1b[90m--------------------------------------------------------------------------------\033[0m\n");

        free(pairs);
    }

    pretty_size ps = prettify(self->size);

    fprintf(stream, "\x1b[97m* heap memory usage summary\033[0m\n\n");
    fprintf(stream, "total memory:     %zu bytes (%.3lf %sbytes)\n", self->size, ps.size, ps.prefix);
    fprintf(stream, "number of chunks: %zu\n", self->num_act);

    fprintf(stream, "\x1b[90m================================================================================\033[0m\n");
}

// --------------------------------------------------------------------------------

global memtable memdbg_tally = DEFAULT_VAL;

void memdbg_reset() {
    memdbg_tally.num_total = memdbg_tally.num_act = memdbg_tally.num_del = 0;
    memdbg_tally.cap = memdbg_tally.size = 0;
    if (memdbg_tally.data != NULL) {
        free(memdbg_tally.data);
        memdbg_tally.data = NULL;
    }
}

bool memdbg_empty() {
    return !memdbg_tally.size && !memdbg_tally.num_act;
}

memdbg_query_output memdbg_query(void *addr) {
    return memtable_get(&memdbg_tally, addr);
}

void memdbg_print_stats(FILE *stream, bool print_chunks) {
    memtable_print_stats(stream, &memdbg_tally, print_chunks);
}

void *memdbg_malloc(usize size, const char *file, i32 line, const char *func) {
    void *ret = malloc(size);

#ifdef MEMDBG_PRINT_ALL
    usize alloc_no = memtable_set(&memdbg_tally, ret, size);
    pretty_size ps = prettify(memdbg_tally.size);

    fprintf(stderr,
            "\x1b[97m[%s:%d]\033[0m in function \x1b[97m'%s'\033[0m \x1b[93mmalloc\033[0m #%zu %zu bytes at %p (total: %.3lf %sbytes)\n",
            file, line, func, alloc_no, size, ret, ps.size, ps.prefix);
#else
    memtable_set(&memdbg_tally, ret, size);
#endif // MEMDBG_PRINT_ALL

    return ret;
}

void *memdbg_calloc(usize num_elems, usize stride, const char *file, i32 line, const char *func) {
    void *ret = calloc(num_elems, stride);

#ifdef MEMDBG_PRINT_ALL
    usize alloc_no = memtable_set(&memdbg_tally, ret, num_elems * stride);
    pretty_size ps = prettify(memdbg_tally.size);

    fprintf(stderr,
            "\x1b[97m[%s:%d]\033[0m in function \x1b[97m'%s'\033[0m \x1b[93mcalloc\033[0m #%zu %zu bytes at %p (total: %.3lf %sbytes)\n",
            file, line, func, alloc_no, num_elems * stride, ret, ps.size, ps.prefix);
#else
    memtable_set(&memdbg_tally, ret, num_elems * stride);
#endif // MEMDBG_PRINT_ALL

    return ret;
}

void *memdbg_realloc(void *ptr, usize size, const char *file, i32 line, const char *func) {
    void *ret = realloc(ptr, size);
    if (ret != ptr) {
        memtable_unset(&memdbg_tally, ptr);
    }

#ifdef MEMDBG_PRINT_ALL
    usize alloc_no = memtable_set(&memdbg_tally, ret, size);
    pretty_size ps = prettify(memdbg_tally.size);

    fprintf(stderr,
            "\x1b[97m[%s:%d]\033[0m in function \x1b[97m'%s'\033[0m \x1b[93mrealloc\033[0m #%zu %zu bytes at %p (total: %.3lf %sbytes)\n",
            file, line, func, alloc_no, size, ret, ps.size, ps.prefix);
#else
    memtable_set(&memdbg_tally, ret, size);
#endif // MEMDBG_PRINT_ALL

    return ret;
}

void memdbg_free(void *ptr, const char *file, i32 line, const char *func) {
    memdbg_query_output output = memtable_get(&memdbg_tally, ptr);

    assert(output.active);

    free(ptr);

    memtable_unset(&memdbg_tally, ptr);

#ifdef MEMDBG_PRINT_ALL
    pretty_size ps = prettify(memdbg_tally.size);
    fprintf(stderr,
            "\x1b[97m[%s:%d]\033[0m in function \x1b[97m'%s'\033[0m \x1b[93mfree\033[0m at %p (total: %.3lf %sbytes)\n",
            file, line, func, ptr, ps.size, ps.prefix);
#endif // MEMDEBUG_PRINT_ALL
}

// --------------------------------------------------------------------------------

#define malloc(size)              memdbg_malloc(size, __FILE__, __LINE__, __func__)
#define calloc(num_elems, stride) memdbg_calloc(num_elems, stride, __FILE__, __LINE__, __func__)
#define realloc(ptr, size)        memdbg_realloc(ptr, size, __FILE__, __LINE__, __func__)
#define free(ptr)                 memdbg_free(ptr, __FILE__, __LINE__, __func__)

// --------------------------------------------------------------------------------

#endif // MEMDBG_H