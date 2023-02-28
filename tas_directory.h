#ifndef TAS_DIRECTORY
#define TAS_DIRECTORY

#include "tas_string.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

StrVec dirListContent(char const * path) {
    StrVec res;
    res.data = NULL;
    res.length = 0;

    DIR *d = opendir(path);

    if(!d) {
        error_exit("Cannot open dir")
    }

    struct dirent *dir;

    while((dir = readdir(d)) != NULL) {
        res.data = (Str *)realloc(res.data, (res.length + 1) * sizeof(Str));
        if(res.data == NULL) {
            error_exit("strSplit error: Cannot reallocate memory for StrVec element\n");
        }
        res.data[res.length] = strNew(path);
        if(!strEndsWithCStr(res.data[res.length], "/")) {
            strPush(&res.data[res.length], '/');
        }
        strAppendCStr(&res.data[res.length], dir->d_name);
        res.length++;
    }

    closedir(d);

    return res;
}

size_t dirPermissionsCStr(char const * path) {
    struct stat stats;
    if(stat(path, &stats) < 0) {
        error_exit("filePermissions error: Cannot read file properties\n");
    }
    return stats.st_mode;
}

size_t dirPermissions(Str str) {
    return dirPermissionsCStr(str.data);
}

void dirSetPermissionsCStr(char const * path, size_t permissions) {
    if(chmod(path, permissions) != 0) {
        error_exit("fileSetPermissionsCStr error: Cannot assign permission to selected file\n");
    }
}

void dirSetPermissions(Str str, size_t permissions) {
    dirSetPermissionsCStr(str.data, permissions);
}

bool dirExisitsCStr(char const * path) {
    return access(path, F_OK) == 0;
}

bool dirExisits(Str path) {
    return access(path.data, F_OK) == 0;
}

#endif