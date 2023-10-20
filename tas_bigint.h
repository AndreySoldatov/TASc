#ifndef TAS_BIGINT
#define TAS_BIGINT

#include <stdio.h>
#include <string.h>

#include "tas_vector.h"
#include "tas_string.h"

VEC(char)

typedef struct BigInt {
    Vec_char data;
} BigInt;

BigInt bigIntNew() {
    BigInt b;
    b.data = vecNew_char();
    return b;
}

BigInt bigIntFromString(char const * str) {
    BigInt b = bigIntNew();

    for (int i = strlen(str) - 1; i >= 0; i--) {
        if(str[i] >= '0' && str[i] <= '9') {
            vecPush_char(&b.data, str[i] - '0');
        } else {
            error_exit("Error bigIntFromString: Provided string contains non digits");
        }
    }

    return b;
}

BigInt bigIntFromInt(int n) {
    BigInt b = bigIntNew();

    while (n > 0) {
        vecPush_char(&b.data, n % 10);
        n /= 10;
    }

    return b;
}

Str bigIntToString(BigInt b) {
    Str s = strNew("");

    for (int i = b.data.length - 1; i >= 0; i--) {
        strPush(&s, b.data.data[i] + '0');
    }

    return s;
}

void bigIntPrint(BigInt b) {
    for (int i = b.data.length - 1; i >= 0; i--) {
        putchar(b.data.data[i] + '0');
    }
}

void bigIntAdd(BigInt *lhs, BigInt rhs) {
    while(lhs->data.length < rhs.data.length) {
        vecPush_char(&lhs->data, 0);
    }
    char carry = 0;
    for (size_t i = 0; i < rhs.data.length; i++) {
        char sum = lhs->data.data[i] + rhs.data.data[i] + carry;
        carry = sum / 10;
        lhs->data.data[i] = sum % 10;
    }
    if(carry) {
        if(lhs->data.length > rhs.data.length)
            lhs->data.data[rhs.data.length] += carry;
        else
            vecPush_char(&lhs->data, carry);
    }
}

void bigIntSub(BigInt *lhs, BigInt rhs) {
    
}

void bigIntDelete(BigInt *b) {
    vecDelete_char(&b->data);
}

#endif