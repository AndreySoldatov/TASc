#include "tas_string.h"
#include "tas_vector.h"

VEC(Str)

void printStr(Str str, size_t i) {
    printf("%lu: ", i);
    strPrintln(str);
}

void deleteStrElem(Str * s, size_t i) {
    strDelete(s);
}

int main() {
    Vec_Str v = vecFrom_Str(2, strNew("Hello World"), strNew("And Github"));
    vecforEach_Str(v, &printStr);
    vecTransformPointer_Str(v, &deleteStrElem);
    vecDelete_Str(&v);
    return 0;
}