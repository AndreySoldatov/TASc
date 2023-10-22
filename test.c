#include <time.h>

#include "tas_bigint.h"

int main() {
    BigInt b = bigIntFromString("600");
    BigInt min = bigIntFromString("500");
    BigInt max = bigIntFromString("1000");

    for (size_t i = 0; i < 20; i++) {
        bigIntRandRange(&b, min, max);
        bigIntPrint(b);
        printf("\n");
    }
}