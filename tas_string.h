#ifndef TAS_STRING
#define TAS_STRING

/**
 * @brief terminates the program and displays error message
 * used in some functions as error handling mechanism
 * 
 */
#define error_exit(msg) printf(msg); exit(1);

/**
 * @brief macro that is used if searching functions failed
*/
#define STR_BAD_INDEX -1

/**
 * @file tas_string.h
 * @brief This file provides a Str data type for dynamically allocated string and some basic manipulation with it
 * @version 0.1
 * @date 2023-02-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "tas_vector.h"
#include "tas_bytebuffer.h"
#include "tas_commonmacro.h"
#include "tas_debugalloc.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

/**
 * String data type
*/
typedef struct Str {
    char * data;
    size_t length;
    size_t capacity;
} Str;

/**
 * Create new str from source (const char *)
 * Example: Str str = strNew("Hello World");
 * After creating str MUST be explicitly deleted:
 * strDelete(str);
*/
Str strNew(char const * source) {
    Str res;
    res.length = strlen(source);
    res.capacity = ((res.length + 1) - ((res.length + 1) % BLOCK_SIZE)) + BLOCK_SIZE;
    res.data = (char *)malloc(res.capacity);
    if(res.data == NULL) {
        error_exit("StrNew error: Cannot allocate memory for str\n")
    }
    strcpy(res.data, source);
    return res;
}

/**
 * This function needs to be called after creating and using a string to avoid memory leaks
*/
void strDelete(Str *str) {
    str->length = 0;
    str->capacity = 0;
    free(str->data);
}

/**
 * put string's content to the standard output
*/
void strPrint(Str str) {
    printf("%s", str.data);
}

void strPrintln(Str str) {
    printf("%s\n", str.data);
}

/**
 * return constant pointer to the string data (const char *)
*/
const char * strData(Str str) {
    return str.data;
}

/**
 * return mutable pointer to the string data (char *)
*/
char * strMutData(Str *str) {
    return str->data;
}

/**
 * return length of string
*/
size_t strLength(Str str) {
    return str.length;
}

void strRequestNewCap(Str * str, size_t newCap) {
    if(newCap > str->capacity) {
        str->capacity = (newCap - (newCap % BLOCK_SIZE)) + BLOCK_SIZE;
        str->data = (char *)realloc(str->data, str->capacity);
    } else if(newCap < str->capacity - BLOCK_SIZE) {
        str->capacity = (newCap - (newCap % BLOCK_SIZE)) + BLOCK_SIZE;
        str->data = (char *)realloc(str->data, str->capacity);
    }
    if(str->data == NULL) {
        error_exit("StrPush error: Cannot Reallocate memory for str\n")
    }
}

/**
 * Push one character to the back of the string
*/
Str * strPush(Str *str, char c) {
    if(str->length + 2 > str->capacity) {
        strRequestNewCap(str, str->length + 2);
    }
    str->data[str->length] = c;
    str->data[str->length + 1] = '\0';
    str->length++;
    return str;
}

/**
 * Push C-style string to the end of destination string
*/
Str * strAppendCStr(Str *dest, char const * source) {
    size_t source_len = strlen(source);
    if(dest->length + source_len + 1 > dest->capacity) {
        strRequestNewCap(dest, dest->length + source_len + 1);
    }
    strcpy(dest->data + dest->length, source);

    dest->length += source_len;

    return dest;
}

/**
 * Appent one string to the back of another
*/
Str * strAppend(Str *dest, Str source) {
    return strAppendCStr(dest, source.data);
}

Str strSub(Str str, size_t first, size_t last) {
    if(first <= last && last - first <= str.length) {
        Str res = strNew("");
        res.length = last - first;
        strRequestNewCap(&res, last - first + 1);
        memcpy(res.data, str.data + first, res.length);
        res.data[res.length] = '\0';
        return res;
    } else {
        error_exit("strSub error: first > last or length of substr > length of str\n")
    }
}

/**
 * Insert string to the *index* of another string
*/
Str * strInsert(Str *dest, Str source, size_t index) {
    if(index < dest->length) {
        Str res = strNew("");
        Str sub1 = strSub(*dest, 0, index);
        strAppend(&res, sub1);
        strAppend(&res, source);
        strDelete(&sub1);
        sub1 = strSub(*dest, index, dest->length);
        strAppend(&res, sub1);
        strDelete(&sub1);

        strDelete(dest);

        dest->data = res.data;
        dest->length = res.length;

        return dest;
    } else {
        error_exit("strInsert error: Index is greater or equal to str length\nif you want to append to str use\nstrAppeng()\n")
    }
}

/**
 * Insert C-string to the *index* of another string
*/
Str * strInsertCStr(Str *dest, char const * source, size_t index) {
    Str tmp = strNew(source);
    dest = strInsert(dest, tmp, index);
    strDelete(&tmp);
    return dest;
}

/**
 * Compare two strings for equality
*/
bool strCompare(Str str1, Str str2) {
    return !strcmp(str1.data, str2.data);
}

/**
 * Compare string and C-String for equality
*/
bool strCompareCStr(Str str1, char const *str2) {
    return !strcmp(str1.data, str2);
}

/**
 * @brief Checks if Str ends with some suffix
 */
bool strEndsWith(Str str1, Str str2) {
    if(str2.length > str1.length) {
        return false;
    }
    Str sub = strSub(str1, str1.length - str2.length, str1.length);
    bool res = strCompare(sub, str2);
    strDelete(&sub);
    return res;
}

/**
 * @brief Checks if Str ends with some C-String suffix
 */
bool strEndsWithCStr(Str str1, char const * str2) {
    Str tmp = strNew(str2);
    bool res = strEndsWith(str1, tmp);
    strDelete(&tmp);
    return res;
}

/**
 * @brief Checks if Str starts with some prefix
 */
bool strStartsWith(Str str1, Str str2) {
    if(str2.length > str1.length) {
        return false;
    }
    Str sub = strSub(str1, 0, str2.length);
    bool res = strCompare(sub, str2);
    strDelete(&sub);
    return res;
}

/**
 * @brief Checks if Str starts with some C-String prefix
 */
bool strStartsWithCStr(Str str1, char const * str2) {
    Str tmp = strNew(str2);
    bool res = strStartsWith(str1, tmp);
    strDelete(&tmp);
    return res;
}

/**
 * Get immutable string element at index
*/
char strAt(Str str, size_t index) {
    if(index >= str.length) {
        error_exit("strAt error: index is greater or equal to the string length\n");
    }
    return str.data[index];
}

/**
 * Set string element at index 
*/
void strSet(Str *str, size_t index, char c) {
    if(index >= str->length) {
        error_exit("strSet error: index is greater or equal to the string length\n");
    }
    str->data[index] = c;
}

/**
 * Find the first occurence of char in string
*/
size_t strFindFirsOf(Str str, char c, size_t start) {
    if(start >= str.length) return STR_BAD_INDEX;
    for (size_t i = start; i < str.length; i++) {
        if(strAt(str, i) == c) {
            return i;
        }
    }
    return STR_BAD_INDEX;
}

/**
 * Find the last occurence of char in string
*/
size_t strFindLastOf(Str  str, char c, size_t start) {
    if(start >= str.length) return STR_BAD_INDEX;
    if(str.length == 0) return STR_BAD_INDEX;
    for (size_t i = str.length - 1 - start; i < str.length; i--) {
        if(strAt(str, i) == c) {
            return i;
        }
    }
    return STR_BAD_INDEX;
}

/**
 * String vector data type; TODO: Impliment basic vector manipulation
 * Use raw contents for now.
*/
typedef struct StrVec {
    Str * data;
    size_t length;
} StrVec;

/**
 * Use this function to clear StrVec
*/
void strVecDelete(StrVec *vec) {
    for (size_t i = 0; i < vec->length; i++) {
        strDelete(&vec->data[i]);
    }
    free(vec->data);
    vec->length = 0;
}

/**
 * @brief Convert C-string array (int size, char ** data) to StrVec
 * WARNING: Copying
 * 
 */
StrVec strVecFromCStrArray(int c, char ** v) {
    StrVec res;
    res.length = c;
    res.data = (Str*)calloc(c, sizeof(Str));
    for (size_t i = 0; i < c; i++) {
        res.data[i] = strNew(v[i]);
    }
    return res;
}

/**
 * @brief Checks if StrVec contains C-style str 
 * 
 */
bool strVecContanins(StrVec v, char const *str) {
    bool res = false;
    for (size_t i = 0; i < v.length; i++) {
        if(strCompareCStr(v.data[i], str)) return true;
    }
    return res;
}

void strClear(Str * str) {
    strDelete(str);
    *str = strNew("");
}

/**
 * Split string using delimiter and return result as StrVec
 * The source string remains untouched
 * WARNING: Copying. If your string is big enough this can be a problem
*/
StrVec strSplit(Str str, char del) {
    StrVec res;
    res.data = NULL;
    res.length = 0;

    size_t start_index = 0;
    size_t end_index = 0;

    while(true) {
        while(start_index < str.length) {
            if(strAt(str, start_index) != del) {
                break;
            }
            start_index++;
        }
        end_index = start_index;
        while(end_index < str.length) {
            if(strAt(str, end_index) == del) {
                break;
            }
            end_index++;
        }

        if(end_index == start_index) break;
        
        res.data = (Str *)realloc(res.data, (res.length + 1) * sizeof(Str));
        if(res.data == NULL) {
            error_exit("strSplit error: Cannot debugReallocate memory for StrVec element");
        }
        res.data[res.length] = strSub(str, start_index, end_index);
        res.length++;

        start_index = end_index;
    }

    return res;
}

/**
 * @brief Convert ASCII string to lowercase
 * 
 */
void strToLowerCase(Str * str) {
    for (size_t i = 0; i < str->length; i++) {
        if(str->data[i] >= 'A' && str->data[i] <= 'Z') {
            str->data[i] += (char)32;
        }
    }
}

/**
 * @brief Convert ASCII string to uppercase
 * 
 */
void strToUpperCase(Str * str) {
    for (size_t i = 0; i < str->length; i++) {
        if(str->data[i] >= 'a' && str->data[i] <= 'z') {
            str->data[i] -= (char)32;
        }
    }
}

/**
 * @brief Converts String to byteBuffer
 * WARNING: Moving
 * 
 */
ByteBuffer strToByteBuffer(Str str) {
    ByteBuffer b;
    b.data = (Byte *)str.data;
    b.length = str.length;
    return b;
}

Str strFiltered(Str str, bool (*pred)(char)) {
    Str res = strNew("");
    for (size_t i = 0; i < str.length; i++) {
        if(!(*pred)(str.data[i])) {
            strPush(&res, str.data[i]);
        }
    }
    return res;
}

#endif