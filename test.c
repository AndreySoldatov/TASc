#include "tas_bigint.h"

int main() {
    BigInt b = bigIntFromString("950");
    BigInt b1 = bigIntFromString("921334");

    bigIntPrint(b);
    printf("\n");
    bigIntPrint(b1);
    printf("\n");

    bigIntAdd(&b, b1);

    bigIntPrint(b);
    printf("\n");
}