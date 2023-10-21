#include "tas_bigint.h"

int main() {
    BigInt b = bigIntFromString("256");
    BigInt b1 = bigIntFromString("7");

    bigIntDiv(&b, b1);

    bigIntPrint(b); // 4154060304
    printf("\n");
}