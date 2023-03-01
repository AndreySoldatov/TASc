#ifndef TAS_ARGS
#define TAS_ARGS

/**
 * @file tas_args.h
 * @brief Super tiny functionality for working with console arguments
 * @version 0.1
 * @date 2023-03-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "tas_string.h"

/**
 * @brief Check if vector of arguments contains the certain flag
 * Flag is defined by "-%flag_name%" expression
 * 
 * @param v Vector of arguments
 * @param flag flag name 
 */
bool argsContainFlag(StrVec v, char const * flag) {
    Str flagStr = strNew("-");
    strAppendCStr(&flagStr, flag);

    bool res = strVecContanins(v, flagStr.data);

    strDelete(&flagStr);

    return res;
}

/**
 * @brief Check if arguments vector contains flag with #-braced message
 * Example -d#/home/soft/#
 *  
 */
bool argsContainFlagWithMessage(StrVec v, char const * flag) {
    Str flagPrefix = strNew("-");
    strAppendCStr(&flagPrefix, flag);
    strAppendCStr(&flagPrefix, "#");
    
    for (size_t i = 0; i < v.length; i++) {
        if(strStartsWith(v.data[i], flagPrefix)) {
            strDelete(&flagPrefix);
            return true;
        }
    }

    strDelete(&flagPrefix);
    return false;    
}

/**
 * @brief Extract #-braced message from flags Vector, given the flag_name
 * 
 */
Str argsGetMessageFromFlag(StrVec v, char const * flag) {
    Str flagPrefix = strNew("-");
    strAppendCStr(&flagPrefix, flag);
    strAppendCStr(&flagPrefix, "#");

    for (size_t i = 0; i < v.length; i++) {
        if(strStartsWith(v.data[i], flagPrefix)) {
            strDelete(&flagPrefix);
            StrVec splited = strSplit(v.data[i], '#');

            Str res;
            if(splited.length > 1) res = strNew(splited.data[1].data);
            else res = strNew("");

            strVecDelete(&splited);
            return res;
        }
    }
    strDelete(&flagPrefix);
    return strNew("");
}

#endif