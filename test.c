#include "tas_bigint.h"

int main() {
    BigInt b = bigIntFromString("236266445788764534654345675467545355634476");

    bigIntSqrt(&b);

    bigIntPrint(b);
}