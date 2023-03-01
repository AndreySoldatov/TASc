#ifndef TAS_DIRECTORY
#define TAS_DIRECTORY

/**
 * @file tas_directory.h
 * @brief This file provides basic directory manipulation functionality
 * @version 0.1
 * @date 2023-02-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "tas_string.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * Enumerates all the directory's children in StrVec.
 * Return value is a FULL PATH
*/
StrVec dirListContentCStr(char const * path) {
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

/**
 * Enumerates all the directory's children in StrVec.
 * Return value is a FULL PATH
*/
StrVec dirListContent(Str str) {
    return dirListContentCStr(str.data);
}

/**
 * Get directory Permissions in mode_t format
 * https://man7.org/linux/man-pages/man2/lstat.2.html
*/
size_t dirPermissionsCStr(char const * path) {
    struct stat stats;
    if(stat(path, &stats) < 0) {
        error_exit("filePermissions error: Cannot read file properties\n");
    }
    return stats.st_mode;
}

/**
 * Get directory Permissions in mode_t format
 * https://man7.org/linux/man-pages/man2/lstat.2.html
*/
size_t dirPermissions(Str str) {
    return dirPermissionsCStr(str.data);
}

/**
 * Set directory Permissions in mode_t format
 * https://man7.org/linux/man-pages/man2/lstat.2.html
*/
void dirSetPermissionsCStr(char const * path, size_t permissions) {
    if(chmod(path, permissions) != 0) {
        error_exit("fileSetPermissionsCStr error: Cannot assign permission to selected file\n");
    }
}

/**
 * Set directory Permissions in mode_t format
 * https://man7.org/linux/man-pages/man2/lstat.2.html
*/
void dirSetPermissions(Str str, size_t permissions) {
    dirSetPermissionsCStr(str.data, permissions);
}

/**
 * Check if directory exists
*/
bool dirExisitsCStr(char const * path) {
    return access(path, F_OK) == 0;
}

/**
 * Check if directory exists
*/
bool dirExisits(Str path) {
    return access(path.data, F_OK) == 0;
}

/**
 * Returns String containing path to the dir's parent directory
 * WARNING: Doesn't work with relative path 
*/
Str dirParentDir(Str str) {
    if(strFindLastOf(str, '/', 0) != STR_BAD_INDEX) {
        return strSub(str, 0, strFindLastOf(str, '/', 0) + 1);
    } else {
        return strNew("");
    }
}

/**
 * Returns String containg dir name from path.
*/
Str dirName(Str str) {
    if(strAt(str, str.length - 1) == '/') {
        return strSub(str, strFindLastOf(str, '/', 1) + 1, str.length - 1);
    } else {
        return strSub(str, strFindLastOf(str, '/', 0) + 0, str.length);
    }
}

/**
 * @brief Create directory, given the path and permissions in mode_t format 
 * 
 */
void dirCreate(char const * path, size_t permissions) {
    mkdir(path, permissions);
}

#endif