#ifndef TAS_FILES
#define TAS_FILES

/**
 * @file tas_files.h
 * @brief This File provides basic file manipulation functions
 * @version 0.1
 * @date 2023-02-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "tas_string.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * Read whole file to the ASCII String
*/
Str fileReadToStr(char const *path) {
    FILE *fp = fopen(path, "r");
    if(fp == NULL) {
        error_exit("fileReadToStr error: cannot open file\n")
    }
    fseek(fp, 0, SEEK_END);
    size_t fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    Str res;
    res.length = fsize;

    res.data = (char *)malloc(fsize + 1);
    fread(res.data, fsize, 1, fp);
    fclose(fp);
    res.data[fsize] = 0;

    return res;
}

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

/**
 * Read whole file to the ByteBuffer object (unsigned char *)
*/
ByteBuffer fileReadByteBuffer(char const *path) {
    FILE *fp = fopen(path, "rb");
    if(fp == NULL) {
        error_exit("fileWriteStr error: cannot open file\n")
    }
    fseek(fp, 0, SEEK_END);
    size_t fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    ByteBuffer res;
    res.length = fsize;
    res.data = (Byte *)malloc(fsize);
    fread(res.data, fsize, 1, fp);
    fclose(fp);

    return res;
}

/**
 * Write C-String to the file. Overrides the previous contents of file. And creates file if nessesary
*/
void fileWriteCStr(char const *path, char const *str) {
    FILE *fp = fopen(path, "w+");
    if(fp == NULL) {
        error_exit("fileWriteStr error: cannot open file\n")
    }
    fprintf(fp, "%s", str);
    fclose(fp);
}

/**
 * Appends C-String to the end of file.
*/
void fileAppendCStr(char const *path, char const *str) {
    FILE *fp = fopen(path, "a+");
    if(fp == NULL) {
        error_exit("fileAppendStr error: cannot open file\n")
    }
    fprintf(fp, "%s", str);
    fclose(fp);
}

/**
 * Write String to the file. Overrides the previous contents of file. And creates file if nessesary
*/
void fileWriteStr(char const *path, Str str) {
    fileWriteCStr(path, str.data);
}

/**
 * Appends String to the end of file.
*/
void fileAppendStr(char const *path, Str str) {
    fileAppendCStr(path, str.data);
}

/**
 * Writes ByteBuffer to file. Overrides previos file content. And creates file if nessesary
*/
void fileWriteByteBuffer(char const *path, ByteBuffer b) {
    FILE *fp = fopen(path, "wb+");
    if(fp == NULL) {
        error_exit("fileWriteByteBuffer error: cannot open file\n")
    }
    fwrite(b.data, b.length, 1, fp);
    fclose(fp);
}

/**
 * Appends ByteBuffer to the end of the file
*/
void fileAppendByteBuffer(char const *path, ByteBuffer b) {
    FILE *fp = fopen(path, "ab+");
    if(fp == NULL) {
        error_exit("fileWriteByteBuffer error: cannot open file\n")
    }
    fwrite(b.data, b.length, 1, fp);
    fclose(fp);
}

/**
 * Returns String containing extension of the file without the dot
*/
Str fileExtension(Str str) {
    if(strFindLastOf(str, '.', 0) != STR_BAD_INDEX) {
        return strSub(str, strFindLastOf(str, '.', 0) + 1, str.length);
    } else {
        return strNew("");
    }
}

/**
 * Returns String containing path to the path's parent directory
 * WARNING: Doesn't work with relative path 
*/
Str fileParentDir(Str str) {
    if(strFindLastOf(str, '/', 0) != STR_BAD_INDEX) {
        return strSub(str, 0, strFindLastOf(str, '/', 0) + 1);
    } else {
        return strNew("");
    }
}

/**
 * Returns String containg file name and extension from path.
*/
Str fileName(Str str) {
    if(strFindLastOf(str, '/', 0) != STR_BAD_INDEX) {
        return strSub(str, strFindLastOf(str, '/', 0) + 1, str.length);
    } else {
        return strNew(str.data);
    }
}

/**
 * Returns file permissions in mode_t format
 * https://man7.org/linux/man-pages/man2/lstat.2.html
*/
size_t filePermissionsCStr(char const *path) {
    struct stat stats;
    if(stat(path, &stats) < 0) {
        error_exit("filePermissions error: Cannot read file properties\n");
    }
    return stats.st_mode;
}

/**
 * Returns file permissions in mode_t format
 * https://man7.org/linux/man-pages/man2/lstat.2.html
*/
size_t filePermissions(Str str) {
    return filePermissionsCStr(str.data);
}

/**
 * Overrides file permissions in mode_t format
 * https://man7.org/linux/man-pages/man2/lstat.2.html
*/
void fileSetPermissionsCStr(char const *path, size_t permissions) {
    if(chmod(path, permissions) != 0) {
        error_exit("fileSetPermissionsCStr error: Cannot assign permission to selected file\n");
    }
}

/**
 * Overrides file permissions in mode_t format
 * https://man7.org/linux/man-pages/man2/lstat.2.html
*/
void fileSetPermissions(Str str, size_t permissions) {
    fileSetPermissionsCStr(str.data, permissions);
}

/**
 * Checks if file exists
*/
bool fileExisitsCStr(char const *path) {
    return access(path, F_OK) == 0;
}

/**
 * Checks if file exists
*/
bool fileExisits(Str path) {
    return access(path.data, F_OK) == 0;
}

#endif