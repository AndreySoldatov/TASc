#ifndef TAS_JSON
#define TAS_JSON

#include "tas_string.h"
#include "tas_unorderedmap.h"
#include "tas_vector.h"

//FIXME: 1. impliment string formatting with \ escape codes support

// TODO: Impliment JsonValue through a union

size_t JSON_DECIMAL_DIGITS = 3;

typedef enum JsonValType {
    JSON_NULL,
    JSON_OBJECT,
    JSON_ARRAY,
    JSON_NUMBER,
    JSON_BOOL,
    JSON_STRING
} JsonValType;

typedef struct JsonValue {
    JsonValType     jsonType;
    double          jsonNumber;
    bool            jsonBool;
    Str             jsonStr;
    struct JsonObject *    jsonObject;
    struct JsonArray *    jsonArray;
} JsonValue;

JsonValue jsonValFromNull() {
    JsonValue res;
    res.jsonArray = NULL;
    res.jsonObject = NULL;
    res.jsonBool = false;
    res.jsonNumber = 0.0;
    res.jsonStr = strNew("");
    res.jsonType = JSON_NULL;
    return res;
}

JsonValue jsonValFromNumber(double val) {
    JsonValue res;
    res.jsonArray = NULL;
    res.jsonObject = NULL;
    res.jsonBool = false;
    res.jsonNumber = val;
    res.jsonStr = strNew("");
    res.jsonType = JSON_NUMBER;
    return res;
}

JsonValue jsonValFromBool(bool val) {
    JsonValue res;
    res.jsonArray = NULL;
    res.jsonObject = NULL;
    res.jsonBool = val;
    res.jsonNumber = 0.0;
    res.jsonStr = strNew("");
    res.jsonType = JSON_BOOL;
    return res;
}

JsonValue jsonValFromStr(char * str) {
    JsonValue res;
    res.jsonArray = NULL;
    res.jsonObject = NULL;
    res.jsonBool = false;
    res.jsonNumber = 0.0;
    res.jsonStr = strNew(str);
    res.jsonType = JSON_STRING;
    return res;
}

bool equals_Str(Str str1, Str str2) {
    return strCompare(str1, str2);
}

UNORDERED_MAP(Str, JsonValue)

typedef uMap_Str_JsonValue JsonObject;

VEC(JsonValue)

typedef Vec_JsonValue JsonArray;

JsonValue jsonValFromArray(JsonArray arr) {
    JsonValue res;
    res.jsonArray = malloc(sizeof(JsonArray));
    *((JsonArray *)res.jsonArray) = arr;
    res.jsonObject = NULL;
    res.jsonBool = false;
    res.jsonNumber = 0.0;
    res.jsonStr = strNew("");
    res.jsonType = JSON_ARRAY;
    return res;
}

JsonValue jsonValFromObject(JsonObject obj) {
    JsonValue res;
    res.jsonArray = NULL;
    res.jsonObject = malloc(sizeof(JsonObject));
    *((JsonObject *)res.jsonObject) = obj;
    res.jsonBool = false;
    res.jsonNumber = 0.0;
    res.jsonStr = strNew("");
    res.jsonType = JSON_OBJECT;
    return res;
}

void jsonObjectDelete(JsonObject * obj);
void jsonArrDelete(JsonArray * arr);

void jsonValDelete(JsonValue * val) {
    strDelete(&val->jsonStr);
    jsonObjectDelete((JsonObject *)val->jsonObject);
    free(val->jsonObject);
    jsonArrDelete((JsonArray *)val->jsonArray);
    free(val->jsonArray);
}

void jsonObjectDelete(JsonObject * obj) {
    if (obj == NULL) return;
    for (size_t i = 0; i < obj->elements.length; i++) {
        strDelete(&obj->elements.data[i].v0);
        jsonValDelete(&obj->elements.data[i].v1);
    }
    uMapDelete_Str_JsonValue(obj);
}

void jsonDelete(JsonValue * val) {
    jsonValDelete(val);
}

void jsonArrDelete(JsonArray * arr) {
    if(arr == NULL) return;
    for (size_t i = 0; i < arr->length; i++) {
        jsonValDelete(&arr->data[i]);
    }
    vecDelete_JsonValue(arr);
}

void jsonDumpValueIndex(JsonValue val, char * del, int index);

void jsonDumpObjectIndex(JsonObject obj, char * del, size_t index);

void jsonDumpArrayIndex(JsonArray arr, char * del, size_t index) {
    for(int i = 0; i < index; i++) printf("%s", del);
    printf("[\n");
    
    for (size_t i = 0; i < arr.length; i++){
        if(arr.data[i].jsonType != JSON_OBJECT && arr.data[i].jsonType != JSON_ARRAY)
            for(int i = 0; i < index + 1; i++) printf("%s", del);
        
        jsonDumpValueIndex(arr.data[i], del, index);

        if(i < arr.length - 1)
            printf(",");
        printf("\n");
    }

    for(int i = 0; i < index; i++) printf("%s", del);
    printf("]");
}

void jsonDumpObjectIndex(JsonObject obj, char * del, size_t index) {
    for(int i = 0; i < index; i++) printf("%s", del);
    printf("{\n");
    for (size_t i = 0; i < obj.elements.length; i++) {
        for(int i = 0; i < index + 1; i++) printf("%s", del);

        printf("\"");
        strPrint(obj.elements.data[i].v0);
        printf("\": ");

        if(obj.elements.data[i].v1.jsonType == JSON_ARRAY || obj.elements.data[i].v1.jsonType == JSON_OBJECT)
            printf("\n");

        jsonDumpValueIndex(obj.elements.data[i].v1, del, index);

        if(i < obj.elements.length - 1)
            printf(",");
        printf("\n");
    }
    for(int i = 0; i < index; i++) printf("%s", del);
    printf("}");
}

void jsonDumpValueIndex(JsonValue val, char * del, int index) {
    if(val.jsonType == JSON_NULL)
        printf("null");
    if(val.jsonType == JSON_NUMBER) {
        Str formatStr = strNew("%.");
        char buff[3];
        sprintf(buff, "%lu", JSON_DECIMAL_DIGITS);
        strAppendCStr(&formatStr, buff);
        strAppendCStr(&formatStr, "f");
        printf(formatStr.data, val.jsonNumber);
        strDelete(&formatStr);
    }
    if(val.jsonType == JSON_BOOL) {
        if(val.jsonBool) {
            printf("true");
        } else printf("false");
    }
    if(val.jsonType == JSON_STRING) {
        printf("\"");
        strPrint(val.jsonStr);
        printf("\"");
    }
    if(val.jsonType == JSON_OBJECT) {
        // printf("\n");
        jsonDumpObjectIndex(*((JsonObject *)val.jsonObject), del, index + 1);
    }
    if(val.jsonType == JSON_ARRAY) {
        // printf("\n");
        jsonDumpArrayIndex(*((JsonArray *)val.jsonArray), del, index + 1);
    }
}

void jsonDump(JsonValue val, char * del) {
    jsonDumpValueIndex(val, del, -1);
}

Str jsonToStringValueIndex(JsonValue val, char * del, int index);

Str jsonToStringArrayIndex(JsonArray arr, char * del, size_t index) {
    Str res = strNew("");

    for(int i = 0; i < index; i++) strAppendCStr(&res, del);
    strAppendCStr(&res, "[\n");
    for (size_t i = 0; i < arr.length; i++) {
        if(arr.data[i].jsonType != JSON_ARRAY && arr.data[i].jsonType != JSON_OBJECT)
            for(int i = 0; i < index + 1; i++) strAppendCStr(&res, del);

        Str tmp = jsonToStringValueIndex(arr.data[i], del, index);
        strAppend(&res, tmp);
        strDelete(&tmp);

        if(i < arr.length - 1)
            strPush(&res, ',');
        strPush(&res, '\n');
    }
    for(int i = 0; i < index; i++) strAppendCStr(&res, del);
    strPush(&res, ']');

    return res;
}

Str jsonToStringObjectIndex(JsonObject obj, char * del, size_t index) {
    Str res = strNew("");

    for(int i = 0; i < index; i++) strAppendCStr(&res, del);
    strAppendCStr(&res, "{\n");
    for (size_t i = 0; i < obj.elements.length; i++) {
        //if(obj.elements.data[i].v1.jsonType != JSON_ARRAY && obj.elements.data[i].v1.jsonType != JSON_OBJECT)
            for(int i = 0; i < index + 1; i++) strAppendCStr(&res, del);
        
        strPush(&res, '\"');
        strAppend(&res, obj.elements.data[i].v0);
        strAppendCStr(&res, "\": ");

        if(obj.elements.data[i].v1.jsonType == JSON_ARRAY || obj.elements.data[i].v1.jsonType == JSON_OBJECT)
            strAppendCStr(&res, "\n");

        Str tmp = jsonToStringValueIndex(obj.elements.data[i].v1, del, index);
        strAppend(&res, tmp);
        strDelete(&tmp);

        if(i < obj.elements.length - 1)
            strPush(&res, ',');
        strPush(&res, '\n');
    }
    for(int i = 0; i < index; i++) strAppendCStr(&res, del);
    strPush(&res, '}');

    return res;
}

Str jsonToStringValueIndex(JsonValue val, char * del, int index) {
    Str res = strNew("");
    
    if(val.jsonType == JSON_NULL)
        strAppendCStr(&res, "null");
    if(val.jsonType == JSON_NUMBER) {
        char buf[BLOCK_SIZE];
        
        Str formatStr = strNew("%.");
        char buff[3];
        sprintf(buff, "%lu", JSON_DECIMAL_DIGITS);
        strAppendCStr(&formatStr, buff);
        strAppendCStr(&formatStr, "f");
        
        sprintf(buf, formatStr.data, val.jsonNumber);
        strAppendCStr(&res, buf);

        strDelete(&formatStr);
    }
    if(val.jsonType == JSON_BOOL) {
        if(val.jsonBool) {
            strAppendCStr(&res, "true");
        } else strAppendCStr(&res, "false");
    }
    if(val.jsonType == JSON_STRING) {
        strPush(&res, '\"');
        strAppend(&res, val.jsonStr);
        strPush(&res, '\"');
    }
    if(val.jsonType == JSON_OBJECT) {
        // strPush(&res, '\n');
        Str tmp = jsonToStringObjectIndex(*((JsonObject *)val.jsonObject), del, index + 1);
        strAppend(&res, tmp);
        strDelete(&tmp);
    }
    if(val.jsonType == JSON_ARRAY) {
        // strPush(&res, '\n');
        Str tmp = jsonToStringArrayIndex(*((JsonArray *)val.jsonArray), del, index + 1);
        strAppend(&res, tmp);
        strDelete(&tmp);
    }

    return res;
}

Str jsonToString(JsonValue val, char * del) {
    Str res = jsonToStringValueIndex(val, del, -1);
    return res;
}

Str smartFiltered(Str str) {
    Str res = strNew("");
    bool canFilter = true;
    for (size_t i = 0; i < str.length; i++) {
        if(str.data[i] == '"') {
            canFilter = !canFilter;
        }

        if(!isWhitespace(str.data[i])) {
            strPush(&res, str.data[i]);
        } else {
            if(!canFilter) {
                strPush(&res, str.data[i]);
            }
        }
    }
    return res;
}

bool isDigit(char c) {
    return (c >= '0' && c <= '9') || c == '.' || c == 'e' || c == 'E';
}

size_t findAdjacentCurvyIndex(Str str, size_t start_index) {
    int brack_count = 0;
    for (size_t i = start_index; i < str.length; i++) {
        if(str.data[i] == '{') {
            brack_count++;
        }
        if(str.data[i] == '}') {
            brack_count--;
            if(brack_count == 0) {
                return i;
            }
        }
    }
    return STR_BAD_INDEX;
}

size_t findAdjacentSquareIndex(Str str, size_t start_index) {
    int brack_count = 0;
    for (size_t i = start_index; i < str.length; i++) {
        if(str.data[i] == '[') {
            brack_count++;
        }
        if(str.data[i] == ']') {
            brack_count--;
            if(brack_count == 0) {
                return i;
            }
        }
    }
    return STR_BAD_INDEX;
}

JsonValue jsonParseValue(Str str, size_t start_index, size_t * end_index);
JsonObject jsonObjectFromString(Str str);

JsonArray jsonArrayFromString(Str str) {
    if(str.data[0] != '[' || str.data[str.length - 1] != ']') {
        error_exit("jsonArrayFromString error: Json Arrayis not valid: no opening or closing bracket\n");
    }
    size_t start_index = 1;
    size_t end_index = 1;
    
    JsonArray res = vecNew_JsonValue();
    
    while(true) {
        JsonValue tmp = jsonParseValue(str, start_index, &end_index);

        vecPush_JsonValue(&res, tmp);

        start_index = end_index;
        if(str.data[start_index] == ',') {
            start_index++;
            continue;
        } else if(str.data[start_index] == ']') {
            break;
        } else {
            error_exit("jsonFromString error: unknown character\n");
        }
    }

    return res;
}

JsonValue jsonParseValue(Str str, size_t start_index, size_t * end_index) {
    if(str.data[start_index] == '\"') { // STRING
        if(strFindFirsOf(str, '\"', start_index + 1) == STR_BAD_INDEX) {
            error_exit("jsonParseValue error: no closing quote on string\n");
        }
        Str val = strSub(str, start_index + 1, strFindFirsOf(str, '\"', start_index + 1));
        //printf("value string: "); strPrintln(val);
        JsonValue res = jsonValFromStr(val.data);
        strDelete(&val);
        *end_index = strFindFirsOf(str, '\"', start_index + 1) + 1;
        return res;
    } else if(str.data[start_index] == 'n') { // NULL
        if(str.length < start_index + 4) {
            error_exit("jsonParseValue error: null init is not complete\n");
        }
        Str nullStr = strSub(str, start_index, start_index + 4);
        if(strCompareCStr(nullStr, "null")) {
            strDelete(&nullStr);
            *end_index = start_index + 4;
            return jsonValFromNull();
        } else {
            error_exit("jsonParseValue error: incorrect null init\n");
        }
    } else if(str.data[start_index] == 't') { // TRUE
        if(str.length < start_index + 4) {
            error_exit("jsonParseValue error: true init is not complete\n");
        }
        Str trueStr = strSub(str, start_index, start_index + 4);
        if(strCompareCStr(trueStr, "true")) {
            strDelete(&trueStr);
            *end_index = start_index + 4;
            return jsonValFromBool(true);
        } else {
            error_exit("jsonParseValue error: incorrect true init\n");
        }
    } else if(str.data[start_index] == 'f') { // FALSE
        if(str.length < start_index + 5) {
            error_exit("jsonParseValue error: false init is not complete\n");
        }
        Str falseStr = strSub(str, start_index, start_index + 5);
        if(strCompareCStr(falseStr, "false")) {
            strDelete(&falseStr);
            *end_index = start_index + 5;
            return jsonValFromBool(false);
        } else {
            error_exit("jsonParseValue error: incorrect false init\n");
        }
    } else if(
        str.data[start_index] == '-' || 
        isDigit(str.data[start_index])) { // NUMBER
        size_t end_idx = start_index + 1;
        
        while(isDigit(str.data[end_idx])) {
            end_idx++;
        }

        Str numStr = strSub(str, start_index, end_idx);
        double val = strtod(numStr.data, NULL);
        *end_index = end_idx;
        strDelete(&numStr);
        return jsonValFromNumber(val);
    } else if(str.data[start_index] == '{') { // OBJECT
        *end_index = findAdjacentCurvyIndex(str, start_index);
        Str objSub = strSub(str, start_index, *end_index + 1);
        *end_index = *end_index + 1;
        JsonObject obj = jsonObjectFromString(objSub);
        strDelete(&objSub);
        return jsonValFromObject(obj);
    } else if(str.data[start_index] == '[') { // ARRAY
        *end_index = findAdjacentSquareIndex(str, start_index);
        Str arrSub = strSub(str, start_index, *end_index + 1);
        *end_index = *end_index + 1;
        JsonArray arr = jsonArrayFromString(arrSub);
        strDelete(&arrSub);
        return jsonValFromArray(arr);
    }
    
    error_exit("jsonParseValue error: Json is not valid: unknown data type\n");
}

JsonObject jsonObjectFromString(Str str) {
    Str workString = smartFiltered(str);
    
    if(workString.data[0] != '{' || workString.data[workString.length - 1] != '}') {
        error_exit("jsonFromString error: Json is not valid: no opening or closing bracket\n");
    }
    
    JsonObject res = uMapNew_Str_JsonValue();
    
    size_t cursor_start = 1;
    size_t cursor_end = 1;

    if(workString.data[cursor_start] == '}') {
        return res;
    }

    while(true) {
        if(workString.data[cursor_start] != '\"') {
            error_exit("jsonFromString error: Json is not valid: expected key of type string\n");
        }

        Str key = strSub(workString, cursor_start + 1, strFindFirsOf(workString, '\"', cursor_start + 1));
        
        cursor_start = strFindFirsOf(workString, '\"', cursor_start + 1) + 1;

        if(workString.data[cursor_start] != ':') {
            error_exit("jsonFromString error: Json is not valid: expected \":\"\n");
        }
        
        cursor_start++;

        JsonValue val = jsonParseValue(workString, cursor_start, &cursor_end);

        uMapSet_Str_JsonValue(&res, key, val);

        cursor_start = cursor_end;

        if(workString.data[cursor_start] == ',') {
            cursor_start++;
            continue;
        } else if(workString.data[cursor_start] == '}') {
            break;
        } else {
            error_exit("jsonFromString error: unknown character\n");
        }
    }
    strDelete(&workString);

    return res;
}

JsonValue jsonFromString(Str str) {
    Str workString = smartFiltered(str);
    size_t end_index = 0;
    JsonValue res = jsonParseValue(workString, 0, &end_index);
    strDelete(&workString);
    return res;
}

#endif