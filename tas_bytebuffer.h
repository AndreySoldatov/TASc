#ifndef TAS_BYTEBUFFER
#define TAS_BYTEBUFFER

/**
 * @file tas_bytebuffer.h
 * @brief Originally part of tas_files, but for the sake of generalisation purpuses
 * was moved here 
 * @version 0.1
 * @date 2023-03-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdlib.h>

typedef unsigned char Byte;

/**
 * Struct for holding an array of bytes
*/
typedef struct ByteBuffer {
    Byte *data;
    size_t length;
} ByteBuffer;

/**
 * Function for deleting byte buffers you MUST call it explicitly for every ByteBuffer object
*/
void byteBufferDelete(ByteBuffer *b) {
    free(b->data);
}

// #include "tas_debugalloc.h"

// void byteBufferAppend(ByteBuffer *b, void * data, size_t length) {
//     b->data = debugRealloc(b->data, b->length + length);
//     memcpy(b->data + b->length, data, length);
//     b->length += length;
// }

#endif