#include "tas_bigint.h"

int main() {
    BigInt b = bigIntFromString("64452");
    BigInt b1 = bigIntFromString("64452");

    bigIntMult(&b, b1);

    bigIntPrint(b); // 4154060304
    printf("\n");
}