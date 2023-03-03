#ifndef TAS_VECTOR
#define TAS_VECTOR

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "tas_commonmacro.h"

#define VecName(T) TYPE_SUFFIX(Vec_, T)

#define VEC(T) typedef struct VecName(T) { \
    T * data; \
    size_t length; \
    size_t capacity; \
} VecName(T); \
VecName(T) TYPE_SUFFIX(vecNew_, T)() { \
    VecName(T) res; \
    res.data = (T *)calloc(BLOCK_SIZE, sizeof(T)); \
    res.length = 0; \
    res.capacity = BLOCK_SIZE; \
    return res; \
} \
void TYPE_SUFFIX(vecDelete_, T)(VecName(T) * v) { \
    free(v->data); \
    v->length = 0; \
} \
void TYPE_SUFFIX(requestNewCap_, T)(VecName(T) * v, size_t size) { \
    if(size > v->capacity) { \
        size_t newCap = (size - (size % BLOCK_SIZE)) + BLOCK_SIZE; \
        v->capacity = newCap; \
        v->data = (T *)realloc(v->data, v->capacity * sizeof(T)); \
    } else if(size < v->capacity - BLOCK_SIZE) { \
        size_t newCap = (size - (size % BLOCK_SIZE)) + BLOCK_SIZE; \
        v->capacity = newCap; \
        v->data = (T *)realloc(v->data, v->capacity * sizeof(T)); \
    } \
} \
void TYPE_SUFFIX(vecPush_, T)(VecName(T) * v, T val) { \
    if(v->length >= v->capacity) { \
        TYPE_SUFFIX(requestNewCap_, T)(v, v->length + 1); \
    } \
    v->data[v->length++] = val; \
} \
void TYPE_SUFFIX(vecPop_, T)(VecName(T) * v) { \
    TYPE_SUFFIX(requestNewCap_, T)(v, v->length - 1); \
    v->length--; \
} \
void TYPE_SUFFIX(vecClear_, T)(VecName(T) * v) { \
    TYPE_SUFFIX(vecDelete_, T)(v); \
    *v = TYPE_SUFFIX(vecNew_, T)(); \
} \
void TYPE_SUFFIX(vecForeach_, T)(VecName(T) v, void (*func)(T, size_t)) { \
    for (size_t i = 0; i < v.length; i++) (*func)(v.data[i], i); \
} \
void TYPE_SUFFIX(vecTransform_, T)(VecName(T) v, T (*func)(T, size_t)) { \
    for (size_t i = 0; i < v.length; i++) v.data[i] = (*func)(v.data[i], i); \
} \
void TYPE_SUFFIX(vecTransformPointer_, T)(VecName(T) v, void (*func)(T *, size_t)) { \
    for (size_t i = 0; i < v.length; i++) (*func)(&v.data[i], i); \
} \
VecName(T) TYPE_SUFFIX(vecFromLambda_, T)(size_t size, T (*func)(size_t)) { \
    VecName(T) res = TYPE_SUFFIX(vecNew_, T)(); \
    for (size_t i = 0; i < size; i++) TYPE_SUFFIX(vecPush_, T)(&res, (*func)(i)); \
    return res; \
} \
VecName(T) TYPE_SUFFIX(vecFrom_, T)(int size, ...) { \
    VecName(T) res = TYPE_SUFFIX(vecNew_, T)(); \
    va_list args; \
    va_start(args, size); \
    for (size_t i = 0; i < size; i++) TYPE_SUFFIX(vecPush_, T)(&res, va_arg(args, T)); \
    va_end(args); \
    return res; \
} \
void TYPE_SUFFIX(vecAppend_, T)(VecName(T) * lhs, VecName(T) rhs) { \
    for (size_t i = 0; i < rhs.length; i++) TYPE_SUFFIX(vecPush_, T)(lhs, rhs.data[i]); \
} \

#endif