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

void bigIntDelete(BigInt *b) {
    vecDelete_char(&b->data);
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

BigInt bigIntCopy(BigInt b) {
    BigInt res = bigIntNew();
    requestNewCap_char(&res.data, b.data.length);
    
    for (size_t i = 0; i < b.data.length; i++) {
        vecPush_char(&res.data, b.data.data[i]);
    }

    return res;
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
    for (size_t i = 0; i < lhs->data.length; i++) {
        char sum;
        if(i < rhs.data.length)
            sum = lhs->data.data[i] + rhs.data.data[i] + carry;
        else
            sum = lhs->data.data[i] + carry;

        carry = sum / 10;
        lhs->data.data[i] = sum % 10;
    }
    if(carry) {
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
    for (size_t i = 0; i < lhs->data.length; i++) {
        if(i < rhs.data.length)
            diff = lhs->data.data[i] - rhs.data.data[i] - debt;
        else
            diff = lhs->data.data[i] - debt;
        
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

void bigIntMultD(BigInt *lhs, char rhs) {
    if(rhs == 0) {
        bigIntDelete(lhs);
        *lhs = bigIntFromString("0");
    }
    char carry = 0;
    for (size_t i = 0; i < lhs->data.length; i++) {
        char mult = lhs->data.data[i] * rhs + carry;
        lhs->data.data[i] = mult % 10;
        carry = mult / 10;
    }
    if(carry) {
        vecPush_char(&lhs->data, carry);
    }
}

void bigIntShiftRight(BigInt *b, int shift) {
    Vec_char tmp = vecNew_char();
    requestNewCap_char(&tmp, shift);
    
    for (size_t i = 0; i < shift; i++) {
        vecPush_char(&tmp, 0);
    }
    
    vecInsertSome_char(&b->data, tmp, 0);

    vecDelete_char(&tmp);
}

void bigIntMult(BigInt * lhs, BigInt rhs) {
    BigInt res = bigIntNew();
    for (size_t i = 0; i < rhs.data.length; i++) {
        BigInt tmp = bigIntCopy(*lhs);
        bigIntMultD(&tmp, rhs.data.data[i]);
        bigIntShiftRight(&tmp, i);
        bigIntAdd(&res, tmp);
    }

    bigIntDelete(lhs);
    lhs->data.capacity = res.data.capacity;
    lhs->data.length = res.data.length;
    lhs->data.data = res.data.data;
}

void bigIntPower(BigInt * lhs, BigInt rhs) {
    BigInt i = bigIntFromString("0");
    BigInt u = bigIntFromString("1");
    BigInt oneLess = bigIntCopy(rhs);
    bigIntSub(&oneLess, u);

    BigInt tmp = bigIntCopy(*lhs);

    for (; bigIntIsSmaller(i, oneLess); bigIntAdd(&i, u)) { // This was fucking painful
        bigIntMult(lhs, tmp);   
    }
}

//I know that this is inefficient but fuck you bitch
void bigIntTrimLeadingZeroes(BigInt * b) {
    while(b->data.data[b->data.length - 1] == 0) {
        vecPop_char(&b->data);
    }
}

// I want to die

void bigIntDiv(BigInt * lhs, BigInt rhs) {
    if(lhs->data.length < rhs.data.length) { // Yo mama would love to choke on ma balls
        bigIntDelete(lhs);
        *lhs = bigIntFromString("0");
        return;
    }

    int length = lhs->data.length - rhs.data.length + 1;
    BigInt res = bigIntNew();
    requestNewCap_char(&res.data, length);
    
    for (size_t i = 0; i < length; i++) {
        vecPush_char(&res.data, 0);
    }
    
    for (int i = length - 1; i >= 0; i--) {
        BigInt tempMult = bigIntCopy(rhs);
        bigIntMult(&tempMult, res);
        while(bigIntIsSmaller(tempMult, *lhs) && res.data.data[i] <= 9) {
            res.data.data[i]++;

            bigIntDelete(&tempMult);
            tempMult = bigIntCopy(rhs);
            bigIntMult(&tempMult, res);
        }
        if(res.data.data[i] != 0 && bigIntRel(tempMult, *lhs) != EQL) res.data.data[i]--;
        bigIntDelete(&tempMult);
    }

    bigIntTrimLeadingZeroes(&res);

    bigIntDelete(lhs);
    lhs->data.capacity = res.data.capacity;
    lhs->data.length = res.data.length;
    lhs->data.data = res.data.data;
}

void bigIntMod(BigInt * lhs, BigInt rhs) {
    BigInt divRes = bigIntCopy(*lhs);
    bigIntDiv(&divRes, rhs);

    bigIntMult(&divRes, rhs);
    bigIntSub(lhs, divRes);

    bigIntDelete(&divRes);
}

void bigIntAverage(BigInt * lhs, BigInt rhs) {
    bigIntAdd(lhs, rhs);
    BigInt two = bigIntFromString("2");
    bigIntDiv(lhs, two);
    bigIntDelete(&two);
}

void bigIntSqrt(BigInt * b) {
    BigInt left = bigIntFromString("0");
    BigInt right = bigIntCopy(*b);
    bigIntAverage(&right, left);

    BigInt middle = bigIntCopy(left);
    bigIntAverage(&middle, right);

    BigInt delta = bigIntCopy(right);
    bigIntSub(&delta, left);

    BigInt u = bigIntFromString("1");

    while(bigIntIsBigger(delta, u)) {
        BigInt middleSquare = bigIntCopy(middle);
        bigIntMult(&middleSquare, middleSquare);
        BigIntRelation rel = bigIntRel(middleSquare, *b);

        switch (rel)
        {
        case BGR:
            bigIntDelete(&right);
            right = bigIntCopy(middle);
            break;
        case SMR:
            bigIntDelete(&left);
            left = bigIntCopy(middle);
            break;
        case EQL:
            bigIntDelete(&middleSquare);
            goto EXIT;
        
        default:
            break;
        }

        bigIntDelete(&middle);
        middle = bigIntCopy(right);
        bigIntAverage(&middle, left);

        bigIntDelete(&delta);
        delta = bigIntCopy(right);
        bigIntSub(&delta, left);
    }

    EXIT:
    bigIntDelete(&left);
    bigIntDelete(&right);
    bigIntDelete(&delta);
    bigIntDelete(&u);
    bigIntDelete(b);
    b->data.capacity = middle.data.capacity;
    b->data.length = middle.data.length;
    b->data.data = middle.data.data;
}

#endif