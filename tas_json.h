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

JsonValue jsonValFromArray(struct JsonArray * arr) {
    JsonValue res;
    res.jsonArray = arr;
    res.jsonObject = NULL;
    res.jsonBool = false;
    res.jsonNumber = 0.0;
    res.jsonStr = strNew("");
    res.jsonType = JSON_ARRAY;
    return res;
}

JsonValue jsonValFromObject(struct JsonObject * obj) {
    JsonValue res;
    res.jsonArray = NULL;
    res.jsonObject = obj;
    res.jsonBool = false;
    res.jsonNumber = 0.0;
    res.jsonStr = strNew("");
    res.jsonType = JSON_OBJECT;
    return res;
}

bool equals_Str(Str str1, Str str2) {
    return strCompare(str1, str2);
}

UNORDERED_MAP(Str, JsonValue)

typedef uMap_Str_JsonValue JsonObject;

VEC(JsonValue)

typedef Vec_JsonValue JsonArray;

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

void jsonDumpIndex(JsonObject obj, char * del, size_t index) {
    for(int i = 0; i < index; i++) printf("%s", del);
    printf("{\n");
    for (size_t i = 0; i < obj.elements.length; i++) {
        for(int i = 0; i < index + 1; i++) printf("%s\"", del);
        strPrint(obj.elements.data[i].v0);
        printf("\": ");

        /// JSON_VAL_PRINT
        if(obj.elements.data[i].v1.jsonType == JSON_NULL)
            printf("NULL");
        if(obj.elements.data[i].v1.jsonType == JSON_NUMBER) 
            printf("%f", obj.elements.data[i].v1.jsonNumber);
        if(obj.elements.data[i].v1.jsonType == JSON_BOOL) {
            if(obj.elements.data[i].v1.jsonBool) {
                printf("true");
            } else printf("false");
        }
        if(obj.elements.data[i].v1.jsonType == JSON_STRING) {
            printf("\"");
            strPrint(obj.elements.data[i].v1.jsonStr);
            printf("\"");
        }
        if(obj.elements.data[i].v1.jsonType == JSON_OBJECT) {
            jsonDumpIndex(obj, del, index + 1);
        }
        if(obj.elements.data[i].v1.jsonType == JSON_ARRAY) {
            //TODO: dumpArrayIndex();
            printf("Array");
        }
        /// JSON_VAL_PRINT

        printf(",\n");
    }
    for(int i = 0; i < index; i++) printf("%s\n", del);
    printf("{\n");
}

void jsonDump(JsonObject obj, char * del) {
    jsonDumpIndex(obj, del, 0);
}

#endif