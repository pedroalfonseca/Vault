#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <float.h>

// --------------------------------------------------------------------------------

typedef unsigned char      ubyte;
typedef unsigned short     ushort;
typedef unsigned int       uint;
typedef unsigned long      ulong;
typedef long long          llong;
typedef unsigned long long ullong;

typedef int8_t             int8;
typedef uint8_t            uint8;
typedef int16_t            int16;
typedef uint16_t           uint16;
typedef int32_t            int32;
typedef uint32_t           uint32;
typedef int64_t            int64;
typedef uint64_t           uint64;

typedef size_t             usize;

typedef intptr_t           intptr;
typedef uintptr_t          uintptr;

typedef ptrdiff_t          ptrdiff;

// --------------------------------------------------------------------------------

#ifdef __cplusplus
#   define DEFAULT_VAL {}
#else
#   define DEFAULT_VAL {0}
#endif // __cplusplus

// --------------------------------------------------------------------------------

#define local    static
#define global   static
#define internal static

// --------------------------------------------------------------------------------

#define introspect(params)

// --------------------------------------------------------------------------------

#endif // TYPES_H