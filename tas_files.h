#ifndef TAS_FILES
#define TAS_FILES

#include "tas_string.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

Str fileReadToStr(char const * path) {
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

typedef struct ByteBuffer {
    Byte * data;
    size_t length;
} ByteBuffer;

void byteBufferDelete(ByteBuffer *b) {
    free(b->data);
}

ByteBuffer fileReadByteBuffer(char const * path) {
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

void fileWriteCStr(char const *path, char const *str) {
    FILE *fp = fopen(path, "w+");
    if(fp == NULL) {
        error_exit("fileWriteStr error: cannot open file\n")
    }
    fprintf(fp, "%s", str);
    fclose(fp);
}

void fileAppendCStr(char const *path, char const *str) {
    FILE *fp = fopen(path, "a+");
    if(fp == NULL) {
        error_exit("fileAppendStr error: cannot open file\n")
    }
    fprintf(fp, "%s", str);
    fclose(fp);
}

void fileWriteStr(char const *path, Str str) {
    fileWriteCStr(path, str.data);
}

void fileAppendStr(char const *path, Str str) {
    fileAppendCStr(path, str.data);
}

void fileWriteByteBuffer(char const *path, ByteBuffer b) {
    FILE *fp = fopen(path, "wb+");
    if(fp == NULL) {
        error_exit("fileWriteByteBuffer error: cannot open file\n")
    }
    fwrite(b.data, b.length, 1, fp);
    fclose(fp);
}

void fileAppendByteBuffer(char const *path, ByteBuffer b) {
    FILE *fp = fopen(path, "ab+");
    if(fp == NULL) {
        error_exit("fileWriteByteBuffer error: cannot open file\n")
    }
    fwrite(b.data, b.length, 1, fp);
    fclose(fp);
}

Str fileExtension(Str str) {
    if(strFindLastOf(str, '.', 0) != STR_BAD_INDEX) {
        return strSub(str, strFindLastOf(str, '.', 0) + 1, str.length);
    } else {
        return strNew("");
    }
}

Str fileParentDir(Str str) {
    if(strFindLastOf(str, '/', 0) != STR_BAD_INDEX) {
        return strSub(str, 0, strFindLastOf(str, '/', 0) + 1);
    } else {
        return strNew("");
    }
}

Str fileName(Str str) {
    if(strFindLastOf(str, '/', 0) != STR_BAD_INDEX) {
        return strSub(str, strFindLastOf(str, '/', 0) + 1, str.length);
    } else {
        return strNew(str.data);
    }
}

size_t filePermissionsCStr(char const * path) {
    struct stat stats;
    if(stat(path, &stats) < 0) {
        error_exit("filePermissions error: Cannot read file properties\n");
    }
    return stats.st_mode;
}

size_t filePermissions(Str str) {
    return filePermissionsCStr(str.data);
}

void fileSetPermissionsCStr(char const * path, size_t permissions) {
    if(chmod(path, permissions) != 0) {
        error_exit("fileSetPermissionsCStr error: Cannot assign permission to selected file\n");
    }
}

void fileSetPermissions(Str str, size_t permissions) {
    fileSetPermissionsCStr(str.data, permissions);
}

bool fileExisitsCStr(char const * path) {
    return access(path, F_OK) == 0;
}

bool fileExisits(Str path) {
    return access(path.data, F_OK) == 0;
}

#endif