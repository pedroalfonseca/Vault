#ifndef MEM_H
#define MEM_H

#include "types.h"

#include <string.h>
#include <assert.h>

// --------------------------------------------------------------------------------

#define is_power_of_two(x)       (((x) & ((x) - 1)) == 0)

// --------------------------------------------------------------------------------

#define bit(x)                   (1 << (x))

#define KB(x)                    (1024ull * (x))
#define MB(x)                    (1024ull * KB(x))
#define GB(x)                    (1024ull * MB(x))

#define line_id(idx)             ((usize)((__LINE__ << 16) | (idx)))

// --------------------------------------------------------------------------------

#define DEFAULT_ARENA_ALIGNMENT  (2 * sizeof(void *))
#define DEFAULT_POOL_ALIGNMENT   8

// --------------------------------------------------------------------------------

internal uintptr align_forward(uintptr ptr, usize alignment) {
    assert(is_power_of_two(alignment));

    uintptr a = (uintptr)alignment;
    // Same as (p % a) but faster as 'a' is a power of two
    uintptr modulo = ptr & (a - 1);

    if (modulo > 0) {
        // If 'ptr' address is not aligned, push the address to the next value which is aligned
        ptr += a - modulo;
    }

    return ptr;
}

// --------------------------------------------------------------------------------

// Adapted from https://www.gingerbill.org/article/2019/02/08/memory-allocation-strategies-002/

typedef struct _arena {
    usize  total_size;
    ubyte *data;

    usize  prev_offset, cur_offset;
} arena;

void arena_clear(arena *self) {
    self->prev_offset = self->cur_offset = 0;
}

void arena_init(arena *self, void *mem, usize total_size) {
    self->total_size = total_size;
    self->data = (ubyte *)mem;
    self->prev_offset = self->cur_offset = 0;
}

void *arena_alloc_align(arena *self, usize size, usize alignment) {
    // Align 'cur_offset' forward to the specified alignment
    uintptr cur_ptr = (uintptr)self->data + (uintptr)self->cur_offset;
    uintptr offset = align_forward(cur_ptr, alignment);
    // Change to relative offset
    offset -= (uintptr)self->data;

    if (offset + size <= self->total_size) {
        void *ptr = self->data + offset;
        self->prev_offset = offset;
        self->cur_offset = offset + size;

        return ptr;
    }

    return NULL;
}

// Because C doesn't have default parameters
void *arena_alloc(arena *self, usize size) {
    return arena_alloc_align(self, size, DEFAULT_ARENA_ALIGNMENT);
}

void arena_free(arena *self, void *ptr) {}

void *arena_resize_align(arena *self, void *mem, usize size, usize new_size, usize alignment) {
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
void *arena_resize(arena *self, void *mem, usize size, usize new_size) {
    return arena_resize_align(self, mem, size, new_size, DEFAULT_ARENA_ALIGNMENT);
}

// --------------------------------------------------------------------------------

typedef struct _tmp_arena {
    arena  *mem;
    usize   prev_offset, cur_offset;
} tmp_arena;

tmp_arena tmp_arena_begin(arena *mem) {
	tmp_arena ret;
	ret.mem = mem;
	ret.prev_offset = mem->prev_offset;
	ret.cur_offset = mem->cur_offset;

	return ret;
}

void tmp_arena_end(tmp_arena tmp) {
	tmp.mem->prev_offset = tmp.prev_offset;
	tmp.mem->cur_offset = tmp.cur_offset;
}

// --------------------------------------------------------------------------------

// Adapted from https://www.gingerbill.org/article/2019/02/16/memory-allocation-strategies-004/

typedef struct _freenode {
    struct _freenode *next;
} freenode;

typedef struct _pool {
    usize     total_size;
    ubyte    *data;

    usize     chunk_size;
    freenode *head;
} pool;

void pool_clear(pool *self) {
    usize num_chunks = self->total_size / self->chunk_size;
    void *ptr = NULL;
    freenode *node = NULL;

    // Set all chunks to be free
    for (usize i = 0; i < num_chunks; ++i) {
        ptr = self->data + (i * self->chunk_size);
        node = (freenode *)ptr;
        // Push free node onto thte free list
        node->next = self->head;
        self->head = node;
    }
}

void pool_init_align(pool *self, void *mem, usize size, usize chunk_size, usize chunk_alignment) {
    // Align backing buffer to the specified chunk alignment
    uintptr initial_start = (uintptr)mem;
    uintptr start = align_forward(initial_start, (uintptr)chunk_alignment);
    size -= (usize)(start - initial_start);

    // Align chunk size up to the required chunk_alignment
    chunk_size = (usize)align_forward(chunk_size, chunk_alignment);

    assert(chunk_size >= sizeof(freenode) && chunk_size <= size);

    self->data = (ubyte *)mem;
    self->total_size = size;
    self->chunk_size = chunk_size;
    self->head = NULL;

    // Set up the free list for free chunks
    pool_clear(self);
}

// Because C doesn't have default parameters
void pool_init(pool *self, void *mem, usize size, usize chunk_size) {
    pool_init_align(self, mem, size, chunk_size, DEFAULT_POOL_ALIGNMENT);
}

void *pool_alloc(pool *self) {
    // Get latest free node
    freenode *node = self->head;
    assert(node != NULL);

    // Pop free node
    self->head = self->head->next;

    return node;
}

void pool_free(pool *self, void *ptr) {
    if (ptr == NULL) {
        return;
    }

    assert(ptr >= (void *)self->data && ptr < (void *)self->data + self->total_size);

    // Push free node
    freenode *node = (freenode *)ptr;
    node->next = self->head;
    self->head = node;
}

// --------------------------------------------------------------------------------

#endif // MEM_H