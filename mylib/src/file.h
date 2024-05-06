#ifndef FILE_H
#define FILE_H

#include "mem.h"

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

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

long file_size(const char *path) {
    assert(path != NULL);

    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        return 0;
    }

    long size = 0;

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);

    fclose(file);

    return size;
}

llong file_timestamp(const char *path) {
    assert(path != NULL);

    struct stat s = DEFAULT_VAL;
    stat(path, &s);

    return s.st_mtime;
}

char *file_read_buffer(const char *path, long *size, char *buffer) {
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

char *file_read(const char *path, long *size, arena *mem) {
    assert(mem != NULL);

    char *data = NULL;
    long _size = file_size(path);

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
    long size = 0;
    char *data = file_read_buffer(src_path, &size, buffer);

    FILE *file = fopen(dst_path, "wb");
    if (file == NULL) {
        return false;
    }

    fclose(file);

    return true;
}

bool file_copy(const char *src_path, const char *dst_path, arena *mem) {
    assert(mem != NULL);

    long size = file_size(src_path);
    if (size > 0) {
        char *buffer = (char *)arena_alloc(mem, size + 1);

        return file_copy_buffer(src_path, dst_path, buffer);
    }

    return false;
}

// --------------------------------------------------------------------------------

#endif // FILE_H