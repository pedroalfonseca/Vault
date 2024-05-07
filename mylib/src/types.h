#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <float.h>

// --------------------------------------------------------------------------------

typedef unsigned char    ubyte;

typedef int8_t           i8;
typedef uint8_t          u8;
typedef int16_t          i16;
typedef uint16_t         u16;
typedef int32_t          i32;
typedef uint32_t         u32;
typedef int64_t          i64;
typedef uint64_t         u64;

typedef float            f32;
typedef double           f64;

typedef ptrdiff_t        isize;
typedef size_t           usize;

typedef intptr_t         iptr;
typedef uintptr_t        uptr;

// --------------------------------------------------------------------------------

#define UBYTE_MAX        255

#define I8_MIN           INT8_MIN
#define I8_MAX           INT8_MAX

#define U8_MAX           UINT8_MAX

#define I16_MIN          INT16_MIN
#define I16_MAX          INT16_MAX

#define U16_MAX          UINT16_MAX

#define I32_MIN          INT32_MIN
#define I32_MAX          INT32_MAX

#define U32_MAX          UINT32_MAX

#define I64_MIN          INT64_MIN
#define I64_MAX          INT64_MAX

#define U64_MAX          UINT64_MAX

#define F32_MIN          FLT_MIN
#define F64_MIN          DBL_MIN

#define ISIZE_MIN        PTRDIFF_MIN
#define ISIZE_MAX        PTRDIFF_MAX

#define USIZE_MAX        SIZE_MAX

#define IPTR_MIN         INTPTR_MIN
#define IPTR_MAX         INTPTR_MAX

#define UPTR_MAX         UINTPTR_MAX

#ifdef __cplusplus
#   define DEFAULT_VAL   {}
#else
#   define DEFAULT_VAL   {0}
#endif // __cplusplus

// --------------------------------------------------------------------------------

#define local            static
#define global           static
#define internal         static

// --------------------------------------------------------------------------------

#define introspect(args)

// --------------------------------------------------------------------------------

#endif // TYPES_H