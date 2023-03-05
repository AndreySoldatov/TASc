#ifndef TAS_DEBUG_ALLOC
#define TAS_DEBUG_ALLOC

#include <stdlib.h>
#include <stdio.h>

size_t malloc_count = 0;
size_t realloc_count = 0;
size_t free_count = 0;

void * debugMalloc(size_t size) {
    printf("%lu allocation: allocating %lu bytes;\n", malloc_count++, size);
    return malloc(size);
}

void * debugCalloc(size_t num, size_t size) {
    printf("%lu allocation: allocating %lu bytes;\n", malloc_count++, num * size);
    return calloc(num, size);
}

void * debugRealloc(void * mem, size_t size) {
    printf("%lu reallocation: reallocating %lu bytes;\n", realloc_count++, size);
    return realloc(mem, size);
}

void debugFree(void * mem) {
    printf("%lu freeing;\n", free_count++);
    free(mem);
}

void debugStats() {
    printf("Allocations: %lu;\nReallocations: %lu;\nFrees: %lu;\n", malloc_count, realloc_count, free_count);
}

#endif