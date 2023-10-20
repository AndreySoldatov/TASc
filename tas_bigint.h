#ifndef TAS_BIGINT
#define TAS_BIGINT

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

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

typedef enum BigIntRelation {
    SMR,
    EQL,
    BGR
} BigIntRelation;

void bigIntRelPrint(BigIntRelation r) {
    switch (r)
    {
    case BGR:
        printf("BGR");
        break;
    case SMR:
        printf("EQL");
        break;
    case EQL:
        printf("EQL");
        break;
    
    default:
        break;
    }
}

Str bigIntRelToString(BigIntRelation r) {
    Str s;
    switch (r)
    {
    case BGR:
        s = strNew("BGR");
        break;
    case SMR:
        s = strNew("EQL");
        break;
    case EQL:
        s = strNew("EQL");
        break;
    
    default:
        break;
    }
    return s;
}

BigIntRelation bigIntRel(BigInt lhs, BigInt rhs) {
    if(lhs.data.length < rhs.data.length) {
        return SMR;
    } else if(lhs.data.length > rhs.data.length) {
        return BGR;
    } else {
        for (int i = lhs.data.length - 1; i >= 0; i--) {
            if(lhs.data.data[i] < rhs.data.data[i]) {
                return SMR;
            } else if(lhs.data.data[i] > rhs.data.data[i]) {
                return BGR;
            } else {
                continue;
            }
        }
        return EQL;
    }
}

bool bigIntIsSmaller(BigInt lhs, BigInt rhs) {
    return bigIntRel(lhs, rhs) == SMR;
}

bool bigIntIsEqual(BigInt lhs, BigInt rhs) {
    return bigIntRel(lhs, rhs) == EQL;
}

bool bigIntIsBigger(BigInt lhs, BigInt rhs) {
    return bigIntRel(lhs, rhs) == BGR;
}

void bigIntSub(BigInt *lhs, BigInt rhs) {
    if(bigIntIsSmaller(*lhs, rhs)) {
        error_exit("Error bigIntSub: Negative numbers are unsupported");
    }

    char debt = 0;
    char diff = 0;
    for (size_t i = 0; i < rhs.data.length; i++) {
        diff = lhs->data.data[i] - rhs.data.data[i] - debt;
        if(diff < 0) {
            debt = 1;
            diff += 10;
        } else {
            debt = 0;
        }
        lhs->data.data[i] = diff;
    }

    while(lhs->data.data[lhs->data.length - 1] == 0) {
        vecPop_char(&lhs->data);
    }
}

void bigIntMult(BigInt * lhs, BigInt rhs) {
    
}

void bigIntDelete(BigInt *b) {
    vecDelete_char(&b->data);
}

#endif