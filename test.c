#include "tas_string.h"
#include "tas_vector.h"
#include "tas_unorderedmap.h"

bool equals(int lhs, int rhs) {
    return lhs == rhs;
}

UNORDERED_MAP(int, char)

void printPair(int k, char v) {
    printf("%d: %c\n", k, v);
}

int main() {
    uMap_int_char m = uMapFrom_int_char(4, 
        pairFrom_int_char(1, 'x'),
        pairFrom_int_char(2, 'y'),
        pairFrom_int_char(3, 'z'),
        pairFrom_int_char(1, 'a')
    );
    uMapForeach_int_char(m, &printPair);
    uMapDelete_int_char(&m);    
}