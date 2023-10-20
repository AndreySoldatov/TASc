#include "tas_bigint.h"

int main() {
    BigInt b = bigIntFromString("5267");
    BigInt b1 = bigIntFromString("2456");

    bigIntPrint(b);
    printf("\n");
    bigIntPrint(b1);
    printf("\n");

    bigIntRelPrint(bigIntRel(b, b1));
    printf("\n");

    bigIntSub(&b, b1);

    bigIntPrint(b);
    printf("\n");

    bigIntRelPrint(bigIntRel(b, b1));
    printf("\n");
}