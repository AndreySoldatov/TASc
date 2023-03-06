#include <stdbool.h>
#include "tas_json.h"
#include "tas_files.h"

int main() {
    Str content = fileReadToStr("test.json");
    JsonObject o = jsonFromString(content);
    jsonDump(o, "   ");
    printf("\n");
    strDelete(&content);
    jsonDelete(&o);
}