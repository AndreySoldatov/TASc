#ifndef TAS_UNORDEREDMAP
#define TAS_UNORDEREDMAP

#include "tas_commonmacro.h"
#include "tas_vector.h"

#define PAIR_SUFFIX(N, T1, T2) TWO_TYPE_SUFFIX(N ## _, T1 ## _, T2)

#define PAIR(T1, T2) typedef struct PAIR_SUFFIX(Pair, T1, T2) { \
    T1 v0; \
    T2 v1; \
} PAIR_SUFFIX(Pair, T1, T2); \
PAIR_SUFFIX(Pair, T1, T2) PAIR_SUFFIX(pairFrom, T1, T2)(T1 v0, T2 v1) { \
    PAIR_SUFFIX(Pair, T1, T2) res; \
    res.v0 = v0; \
    res.v1 = v1; \
    return res; \
} \

#define PAIR_VECTOR(T1, T2) VecName(PAIR_SUFFIX(Pair, T1, T2))
#define MAP_NAME(T1, T2) PAIR_SUFFIX(uMap, T1, T2)
#define FUNCTION_WITH_PAIR(N, T1, T2) N ## _ ## Pair_ ## T1 ## _ ## T2

#define UNORDERED_MAP(T1, T2) PAIR(T1, T2) \
VEC(PAIR_SUFFIX(Pair, T1, T2)) \
typedef struct MAP_NAME(T1, T2) { \
    PAIR_VECTOR(T1, T2) elements; \
} MAP_NAME(T1, T2); \
MAP_NAME(T1, T2) PAIR_SUFFIX(uMapNew, T1, T2)() { \
    MAP_NAME(T1, T2) res; \
    res.elements = FUNCTION_WITH_PAIR(vecNew, T1, T2)(); \
    return res; \
} \
void PAIR_SUFFIX(uMapDelete, T1, T2)(MAP_NAME(T1, T2) * map) { \
    FUNCTION_WITH_PAIR(vecDelete, T1, T2)(&map->elements); \
} \
T2 * PAIR_SUFFIX(uMapAt, T1, T2)(MAP_NAME(T1, T2) map, T1 key) { \
    for (size_t i = 0; i < map.elements.length; i++) { \
        if(equals(map.elements.data[i].v0, key)) { \
            return &map.elements.data[i].v1; \
        } \
    } \
    return NULL; \
} \
bool PAIR_SUFFIX(uMapContains, T1, T2)(MAP_NAME(T1, T2) map, T1 key) { \
    for (size_t i = 0; i < map.elements.length; i++) { \
        if(equals(map.elements.data[i].v0, key)) { \
            return true; \
        } \
    } \
    return false; \
} \
void PAIR_SUFFIX(uMapSet, T1, T2)(MAP_NAME(T1, T2) * map, T1 key, T2 val) { \
    if(PAIR_SUFFIX(uMapContains, T1, T2)(*map, key)) { \
        T2 * val_ptr = PAIR_SUFFIX(uMapAt, T1, T2)(*map, key); \
        *val_ptr = val; \
    } else { \
        PAIR_SUFFIX(Pair, T1, T2) e; \
        e.v0 = key; \
        e.v1 = val; \
        FUNCTION_WITH_PAIR(vecPush, T1, T2)(&(map->elements), e); \
    } \
} \
MAP_NAME(T1, T2) PAIR_SUFFIX(uMapFrom, T1, T2)(int size, ...) { \
    MAP_NAME(T1, T2) res = PAIR_SUFFIX(uMapNew, T1, T2)(); \
    va_list args; \
    va_start(args, size); \
    for (size_t i = 0; i < size; i++) { \
        PAIR_SUFFIX(Pair, T1, T2) tmp = va_arg(args, PAIR_SUFFIX(Pair, T1, T2)); \
        PAIR_SUFFIX(uMapSet, T1, T2)(&res, tmp.v0, tmp.v1); \
    } \
    va_end(args); \
    return res; \
} \
void PAIR_SUFFIX(uMapForeach, T1, T2)(MAP_NAME(T1, T2) map, void (*func)(T1, T2)) { \
    for (size_t i = 0; i < map.elements.length; i++) \
        (*func)(map.elements.data[i].v0, map.elements.data[i].v1); \
} \

#endif