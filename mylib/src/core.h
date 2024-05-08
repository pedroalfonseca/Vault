#ifndef CORE_H
#define CORE_H

#include "types.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>

// --------------------------------------------------------------------------------

#define is_power_of_two(x)       (((x) & ((x) - 1)) == 0)

// --------------------------------------------------------------------------------

#define bit(x)                   (1 << (x))

#define KB(x)                    (1024ull * (x))
#define MB(x)                    (1024ull * KB(x))
#define GB(x)                    (1024ull * MB(x))

#define line_id(idx)             ((usize)((__LINE__ << 16) | (idx)))

// --------------------------------------------------------------------------------

#define ARENA_DEFAULT_ALIGNMENT  (2 * sizeof(void *))
#define POOL_DEFAULT_ALIGNMENT   8

internal uptr align_forward(uptr ptr, usize alignment) {
    assert(is_power_of_two(alignment));

    uptr a = (uptr)alignment;
    // Same as (p % a) but faster as 'a' is a power of two
    uptr modulo = ptr & (a - 1);

    if (modulo > 0) {
        // If 'ptr' address is not aligned, push the address to the next value which is aligned
        ptr += a - modulo;
    }

    return ptr;
}

// --------------------------------------------------------------------------------

// Adapted from https://www.gingerbill.org/article/2019/02/08/memory-allocation-strategies-002/

typedef struct _Arena {
    usize  total_size;
    ubyte *data;

    usize  prev_offset, cur_offset;
} Arena;

void arena_clear(Arena *self) {
    self->prev_offset = self->cur_offset = 0;
}

void arena_init(Arena *self, void *mem, usize total_size) {
    self->total_size = total_size;
    self->data = (ubyte *)mem;
    self->prev_offset = self->cur_offset = 0;
}

void *arena_alloc_align(Arena *self, usize size, usize alignment) {
    // Align 'cur_offset' forward to the specified alignment
    uptr cur_ptr = (uptr)self->data + (uptr)self->cur_offset;
    uptr offset = align_forward(cur_ptr, alignment);
    // Change to relative offset
    offset -= (uptr)self->data;

    if (offset + size <= self->total_size) {
        void *ptr = self->data + offset;
        self->prev_offset = offset;
        self->cur_offset = offset + size;

        return ptr;
    }

    return NULL;
}

// Because C doesn't have default parameters
void *arena_alloc(Arena *self, usize size) {
    return arena_alloc_align(self, size, ARENA_DEFAULT_ALIGNMENT);
}

void arena_free(Arena *self, void *ptr) {}

void *arena_resize_align(Arena *self, void *mem, usize size, usize new_size, usize alignment) {
    ubyte *old_mem = (ubyte *)mem;
    usize old_size = size;

    assert(is_power_of_two(alignment));

    if (!old_mem || !old_size) {
        return arena_alloc_align(self, new_size, alignment);
    }

    if (self->data <= old_mem && old_mem < self->data + self->total_size) {
        if (self->data + self->prev_offset == old_mem) {
            self->cur_offset = self->prev_offset + new_size;

            return old_mem;
        }

        void *new_mem = arena_alloc_align(self, new_size, alignment);
        usize copy_size = old_size < new_size ? old_size : new_size;
        // Copy across old memory to the new memory
        memmove(new_mem, old_mem, copy_size);

        return new_mem;
    }

    return NULL;
}

// Because C doesn't have default parameters
void *arena_resize(Arena *self, void *mem, usize size, usize new_size) {
    return arena_resize_align(self, mem, size, new_size, ARENA_DEFAULT_ALIGNMENT);
}

// --------------------------------------------------------------------------------

typedef struct _Tmp_Arena {
    Arena  *mem;
    usize   prev_offset, cur_offset;
} Tmp_Arena;

Tmp_Arena tmp_arena_begin(Arena *mem) {
	Tmp_Arena ret;
	ret.mem = mem;
	ret.prev_offset = mem->prev_offset;
	ret.cur_offset = mem->cur_offset;

	return ret;
}

void tmp_arena_end(Tmp_Arena tmp) {
	tmp.mem->prev_offset = tmp.prev_offset;
	tmp.mem->cur_offset = tmp.cur_offset;
}

// --------------------------------------------------------------------------------

// Adapted from https://www.gingerbill.org/article/2019/02/16/memory-allocation-strategies-004/

typedef struct _Freenode {
    struct _Freenode *next;
} Freenode;

typedef struct _Pool {
    usize     total_size;
    ubyte    *data;

    usize     chunk_size;
    Freenode *head;
} Pool;

void pool_clear(Pool *self) {
    usize num_chunks = self->total_size / self->chunk_size;
    void *ptr = NULL;
    Freenode *node = NULL;

    // Set all chunks to be free
    for (usize i = 0; i < num_chunks; ++i) {
        ptr = self->data + (i * self->chunk_size);
        node = (Freenode *)ptr;
        // Push free node onto thte free list
        node->next = self->head;
        self->head = node;
    }
}

void pool_init_align(Pool *self, void *mem, usize size, usize chunk_size, usize chunk_alignment) {
    // Align backing buffer to the specified chunk alignment
    uptr initial_start = (uptr)mem;
    uptr start = align_forward(initial_start, (uptr)chunk_alignment);
    size -= (usize)(start - initial_start);

    // Align chunk size up to the required chunk alignment
    chunk_size = (usize)align_forward(chunk_size, chunk_alignment);

    assert(chunk_size >= sizeof(Freenode) && chunk_size <= size);

    self->data = (ubyte *)mem;
    self->total_size = size;
    self->chunk_size = chunk_size;
    self->head = NULL;

    // Set up the free list for free chunks
    pool_clear(self);
}

// Because C doesn't have default parameters
void pool_init(Pool *self, void *mem, usize size, usize chunk_size) {
    pool_init_align(self, mem, size, chunk_size, POOL_DEFAULT_ALIGNMENT);
}

void *pool_alloc(Pool *self) {
    // Get latest free node
    Freenode *node = self->head;
    assert(node != NULL);

    // Pop free node
    self->head = self->head->next;

    return node;
}

void pool_free(Pool *self, void *ptr) {
    if (ptr == NULL) {
        return;
    }

    assert((ubyte *)ptr >= self->data && (ubyte *)ptr < self->data + self->total_size);

    // Push free node
    Freenode *node = (Freenode *)ptr;
    node->next = self->head;
    self->head = node;
}

// --------------------------------------------------------------------------------

#define countof(a)             (sizeof((a)) / sizeof(*(a)))

// --------------------------------------------------------------------------------

// Adapted from https://github.com/nothings/stb/blob/master/stb_ds.h

#define array_capacity(a)      ((a) ? _array_capacity(a) : 0)
#define array_count(a)         ((a) ? _array_count(a) : 0)
#define array_resize(a, n)     (((n) > array_capacity(a) ? _array_grow(a, 0, n), 1 : 1), (a) ? _array_count(a) = (n), (a) : NULL)
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

bool file_exists(const char *path) {
    assert(path != NULL);

    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        return false;
    }

    fclose(file);

    return true;
}

i32 file_size(const char *path) {
    assert(path != NULL);

    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        return 0;
    }

    i32 size = 0;

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);

    fclose(file);

    return size;
}

i64 file_timestamp(const char *path) {
    assert(path != NULL);

    struct stat s = DEFAULT_VAL;
    stat(path, &s);

    return s.st_mtime;
}

char *file_read_buffer(const char *path, i32 *size, char *buffer) {
    assert(path != NULL);
    assert(size != NULL);
    assert(buffer != NULL);

    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        return NULL;
    }

    *size = 0;

    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    fseek(file, 0, SEEK_SET);

    memset(buffer, 0, *size + 1); // TODO(paalf): is this really necessary?
    fread(buffer, sizeof(char), *size, file);

    fclose(file);

    return buffer;
}

char *file_read(const char *path, i32 *size, Arena *mem) {
    assert(mem != NULL);

    char *data = NULL;
    i32 _size = file_size(path);

    if (_size > 0) {
        char *buffer = (char *)arena_alloc(mem, _size + 1);

        data = file_read_buffer(path, size, buffer);
    }

    return data;
}

bool file_write(const char *path, char *buffer, usize count) {
    assert(path != NULL);
    assert(buffer != NULL);

    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        return false;
    }

    fwrite(buffer, sizeof(char), count, file);

    fclose(file);

    return true;
}

bool file_copy_buffer(const char *src_path, const char *dst_path, char *buffer) {
    i32 size = 0;
    char *data = file_read_buffer(src_path, &size, buffer);

    FILE *file = fopen(dst_path, "wb");
    if (file == NULL) {
        return false;
    }

    fclose(file);

    return true;
}

bool file_copy(const char *src_path, const char *dst_path, Arena *mem) {
    assert(mem != NULL);

    i32 size = file_size(src_path);
    if (size > 0) {
        char *buffer = (char *)arena_alloc(mem, size + 1);

        return file_copy_buffer(src_path, dst_path, buffer);
    }

    return false;
}

// --------------------------------------------------------------------------------

typedef enum _Text_Color {
    TEXT_COLOR_RED,
    TEXT_COLOR_GREEN,
    TEXT_COLOR_YELLOW,
    TEXT_COLOR_COUNT
} Text_Color;

void _log(FILE *stream, const char *prefix, const char *fmt, Text_Color color, ...) {
    local const char *text_color_table[TEXT_COLOR_COUNT] = {
        "\x1b[31m", // TEXT_COLOR_RED
        "\x1b[32m", // TEXT_COLOR_GREEN
        "\x1b[33m", // TEXT_COLOR_YELLOW
    };

    char fmt_buffer[128];
    sprintf(fmt_buffer, "%s%s:\033[0m %s\n", text_color_table[color], prefix, fmt);

    local char buffer[128];
    va_list args;
    va_start(args, color);
    vsprintf(buffer, fmt_buffer, args);
    va_end(args);

    fputs(buffer, stream);
}

extern FILE *_log_file;

#ifdef DEBUG_MODE
#   define log_trace(fmt, ...)   _log(stderr, "TRACE", fmt, TEXT_COLOR_GREEN, ##__VA_ARGS__)
#   define log_warning(fmt, ...) _log(stderr, "WARNING", fmt, TEXT_COLOR_YELLOW, ##__VA_ARGS__)
#   define log_error(fmt, ...)   _log(stderr, "ERROR", fmt, TEXT_COLOR_RED, ##__VA_ARGS__)
#else
#   define log_trace(fmt, ...)   _log(_log_file, "TRACE", fmt, TEXT_COLOR_GREEN, ##__VA_ARGS__)
#   define log_warning(fmt, ...) _log(_log_file, "WARNING", fmt, TEXT_COLOR_YELLOW, ##__VA_ARGS__)
#   define log_error(fmt, ...)   _log(_log_file, "ERROR", fmt, TEXT_COLOR_RED, ##__VA_ARGS__)
#endif // DEBUG_MODE

// --------------------------------------------------------------------------------

#define _concat(x, y) x##y
#define concat(x, y)  _concat(x, y)

// --------------------------------------------------------------------------------

#if defined(__cplusplus) && (__cplusplus >= 201103L)

template<typename T> struct Remove_Reference       { typedef T Type; };
template<typename T> struct Remove_Reference<T &>  { typedef T Type; };
template<typename T> struct Remove_Reference<T &&> { typedef T Type; };

// --------------------------------------------------------------------------------

template<typename T> inline T &&forward(typename Remove_Reference<T>::Type &t)  { return static_cast<T &&>(t); }
template<typename T> inline T &&forward(typename Remove_Reference<T>::Type &&t) { return static_cast<T &&>(t); }
template<typename T> inline T &&move(T &&t)                                     { return static_cast<typename Remove_Reference<T>::Type &&>(t); }

// --------------------------------------------------------------------------------

template<typename Func>
struct Scope_Exit {
    Func function;

    Scope_Exit(Func &&func) : function(forward<Func>(func)) {}

    ~Scope_Exit() { function(); }
};

struct Scope_Exit_Helper {
    template<typename Func> Scope_Exit<Func> operator<<(Func &&func) { return Scope_Exit<Func>(forward<Func>(func)); }
};

#define defer const auto &concat(_defer_, __LINE__) = Scope_Exit_Helper() << [&]()

#endif // defined(__cplusplus) && (__cplusplus >= 201103L)

// --------------------------------------------------------------------------------

#endif // CORE_H