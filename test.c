#include <stdbool.h>
#include "tas_json.h"
#include "tas_files.h"

int main() {
    JsonObject json = uMapNew_Str_JsonValue();
    uMapSet_Str_JsonValue(&json, strNew("PI"), jsonValFromNumber(3.1415));
    uMapSet_Str_JsonValue(&json, strNew("life"), jsonValFromBool(true));
    uMapSet_Str_JsonValue(&json, strNew("hello"), jsonValFromStr("World"));
    uMapSet_Str_JsonValue(&json, strNew("meaning_of_life"), jsonValFromNull());
    JsonObject sub = uMapNew_Str_JsonValue();
    uMapSet_Str_JsonValue(&sub, strNew("sub1"), jsonValFromNumber(3.1415));
    uMapSet_Str_JsonValue(&sub, strNew("sub2"), jsonValFromBool(true));
    JsonArray arr = vecFrom_JsonValue(3,
        jsonValFromNumber(120.),
        jsonValFromStr("I am number 130.000"),
        jsonValFromNumber(140.)
    );
    uMapSet_Str_JsonValue(&sub, strNew("Arr"), jsonValFromArray(&arr));
    uMapSet_Str_JsonValue(&json, strNew("sub"), jsonValFromObject(&sub)); 
    
    Str res = jsonToString(json, "   ");
    strPrintln(res);

    fileWriteStr("res.json", res);

    strDelete(&res);
    
    jsonDelete(&json);
}