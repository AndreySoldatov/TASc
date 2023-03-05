#ifndef TAS_JSON
#define TAS_JSON

#include "tas_string.h"
#include "tas_unorderedmap.h"
#include "tas_vector.h"

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

JsonValue jsonValFromArray(JsonArray * arr) {
    JsonValue res;
    res.jsonArray = (struct JsonArray *)arr;
    res.jsonObject = NULL;
    res.jsonBool = false;
    res.jsonNumber = 0.0;
    res.jsonStr = strNew("");
    res.jsonType = JSON_ARRAY;
    return res;
}

JsonValue jsonValFromObject(JsonObject * obj) {
    JsonValue res;
    res.jsonArray = NULL;
    res.jsonObject = (struct JsonObject *)obj;
    res.jsonBool = false;
    res.jsonNumber = 0.0;
    res.jsonStr = strNew("");
    res.jsonType = JSON_OBJECT;
    return res;
}

void jsonDelete(JsonObject * obj);
void jsonArrDelete(JsonArray * arr);

void jsonValDelete(JsonValue * val) {
    strDelete(&val->jsonStr);
    jsonDelete((JsonObject *)val->jsonObject);
    jsonArrDelete((JsonArray *)val->jsonArray);
}

void jsonDelete(JsonObject * obj) {
    if (obj == NULL) return;
    for (size_t i = 0; i < obj->elements.length; i++) {
        strDelete(&obj->elements.data[i].v0);
        jsonValDelete(&obj->elements.data[i].v1);
    }
    uMapDelete_Str_JsonValue(obj);
}

void jsonArrDelete(JsonArray * arr) {
    if(arr == NULL) return;
    for (size_t i = 0; i < arr->length; i++) {
        jsonValDelete(&arr->data[i]);
    }
    vecDelete_JsonValue(arr);
}

void jsonDumpValueIndex(JsonValue val, char * del, size_t index);

void jsonDumpIndex(JsonObject obj, char * del, size_t index);

void jsonDumpArrayIndex(JsonArray arr, char * del, size_t index) {
    for(int i = 0; i < index; i++) printf("%s", del);
    printf("[\n");
    
    for (size_t i = 0; i < arr.length; i++){
        for(int i = 0; i < index + 1; i++) printf("%s", del);
        
        jsonDumpValueIndex(arr.data[i], del, index);

        if(i < arr.length - 1)
            printf(",");
        printf("\n");
    }

    for(int i = 0; i < index; i++) printf("%s", del);
    printf("]");
}

void jsonDumpIndex(JsonObject obj, char * del, size_t index) {
    for(int i = 0; i < index; i++) printf("%s", del);
    printf("{\n");
    for (size_t i = 0; i < obj.elements.length; i++) {
        for(int i = 0; i < index + 1; i++) printf("%s", del);
        printf("\"");
        strPrint(obj.elements.data[i].v0);
        printf("\": ");

        jsonDumpValueIndex(obj.elements.data[i].v1, del, index);

        if(i < obj.elements.length - 1)
            printf(",");
        printf("\n");
    }
    for(int i = 0; i < index; i++) printf("%s", del);
    printf("}");
}

void jsonDumpValueIndex(JsonValue val, char * del, size_t index) {
    if(val.jsonType == JSON_NULL)
        printf("null");
    if(val.jsonType == JSON_NUMBER) 
        printf("%f", val.jsonNumber);
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
        printf("\n");
        jsonDumpIndex(*((JsonObject *)val.jsonObject), del, index + 1);
    }
    if(val.jsonType == JSON_ARRAY) {
        printf("\n");
        jsonDumpArrayIndex(*((JsonArray *)val.jsonArray), del, index + 1);
    }
}

void jsonDump(JsonObject obj, char * del) {
    jsonDumpIndex(obj, del, 0);
}

Str jsonToStringValueIndex(JsonValue val, char * del, size_t index);

Str jsonToStringArrayIndex(JsonArray arr, char * del, size_t index) {
    Str res = strNew("");

    for(int i = 0; i < index; i++) strAppendCStr(&res, del);
    strAppendCStr(&res, "[\n");
    for (size_t i = 0; i < arr.length; i++) {
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

Str jsonToStringIndex(JsonObject obj, char * del, size_t index) {
    Str res = strNew("");

    for(int i = 0; i < index; i++) strAppendCStr(&res, del);
    strAppendCStr(&res, "{\n");
    for (size_t i = 0; i < obj.elements.length; i++) {
        for(int i = 0; i < index + 1; i++) strAppendCStr(&res, del);
        strPush(&res, '\"');
        strAppend(&res, obj.elements.data[i].v0);
        strAppendCStr(&res, "\": ");

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

Str jsonToStringValueIndex(JsonValue val, char * del, size_t index) {
    Str res = strNew("");
    
    if(val.jsonType == JSON_NULL)
        strAppendCStr(&res, "null");
    if(val.jsonType == JSON_NUMBER) {
        char buf[BLOCK_SIZE]; 
        sprintf(buf, "%f", val.jsonNumber);
        strAppendCStr(&res, buf);
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
        strPush(&res, '\n');
        Str tmp = jsonToStringIndex(*((JsonObject *)val.jsonObject), del, index + 1);
        strAppend(&res, tmp);
        strDelete(&tmp);
    }
    if(val.jsonType == JSON_ARRAY) {
        strPush(&res, '\n');
        Str tmp = jsonToStringArrayIndex(*((JsonArray *)val.jsonArray), del, index + 1);
        strAppend(&res, tmp);
        strDelete(&tmp);
    }

    return res;
}

Str jsonToString(JsonObject obj, char * del) {
    Str res = jsonToStringIndex(obj, del, 0);
    return res;
}

bool isWhitespace(char c) {
    return c == ' ' || c == '\n' || c == '\t' || c == (char)13;
}

JsonValue jsonParseValue(Str str, size_t start_index, size_t * end_index) {
    if(workString.data[cursor_start] == '\"') { // STRING
        if(strFindFirsOf(workString, '\"', cursor_start + 1) == STR_BAD_INDEX) {
            error_exit("jsonParseValue error: no closing quote on string\n");
        }
        Str val = strSub(workString, cursor_start, strFindFirsOf(workString, '\"', cursor_start + 1));
        printf("value string: "); strPrintln(val);
        JsonValue res = jsonValFromStr(val.data);
        strDelete(val);
        return res;
    } else if(workString.data[cursor_start] == 'n') { // NULL
        //FIXME: Want to sleep
        Str nullStr = 
        if()
    } else if(workString.data[cursor_start] == 't') { // TRUE
        //TODO: TRUE
    } else if(workString.data[cursor_start] == 'f') { // FALSE
        //TODO: FALSE
    } else if(
        workString.data[cursor_start] == '-' || 
        isDigit(workString.data[cursor_start])) { // NUMBER
        //TODO: NUMBER
    } else if(workString.data[cursor_start] == '{') { // OBJECT
        //TODO: OBJECT
    } else if(workString.data[cursor_start] == '[') { // ARRAY
        //TODO: ARRAY
    } else {
        error_exit("jsonParseValue error: Json is not valid: unknown data type\n");
    }
}

JsonObject jsonFromString(Str str) {
    Str workString = strFiltered(str, &isWhitespace);
    strPrintln(workString);
    
    if(workString.data[0] != '{' || workString.data[workString.length - 1] != '}') {
        error_exit("jsonFromString error: Json is not valid: no opening or closing bracket\n");
    }
    
    JsonObject res = uMapNew_Str_JsonValue();
    
    size_t cursor_start = 1;
    if(workString.data[cursor_start] == '}') {
        return res;
    }

    while(true) {
        if(workString.data[cursor_start] != '\"') {
            error_exit("jsonFromString error: Json is not valid: expected key of type string\n");
        }

        Str key = strSub(workString, cursor_start, strFindFirsOf(workString, '\"', cursor_start + 1));
        printf("Key: "); strPrintln(key);
        
        cursor_start = strFindFirsOf(workString, '\"', cursor_start) + 1;
        if(workString.data[cursor_start] != ':') {
            error_exit("jsonFromString error: Json is not valid: expected \":\"\n");
        }
        
        cursor_start++;
    }
    strDelete(&workString);
}

#endif