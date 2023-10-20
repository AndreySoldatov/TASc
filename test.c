#include "tas_bigint.h"

int main() {
    BigInt b = bigIntFromString("2");
    BigInt b1 = bigIntFromString("10000");

    bigIntPower(&b, b1);

    bigIntPrint(b); // 4154060304
    printf("\n");
}