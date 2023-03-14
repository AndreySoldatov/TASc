#ifndef TAS_COMMONMACRO
#define TAS_COMMONMACRO

#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 32
#define TYPE_SUFFIX(N, T) N ## T
#define TWO_TYPE_SUFFIX(N, T1, T2) N ## T1 ## T2
#define error_exit(msg) printf(msg); exit(1);

#endif