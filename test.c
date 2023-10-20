#include "tas_bigint.h"

int main() {
    Vec_char c = vecNew_char();
    vecPush_char(&c, 'a');
    vecPush_char(&c, 'b');
    vecPush_char(&c, 'd');
    vecPush_char(&c, 'e');

    for (size_t i = 0; i < c.length; i++) {
        printf("%c, ", c.data[i]);
    }
    printf("\n");

    printf("%d\n", c.length);
    
    vecInsertOne_char(&c, 'c', 2);
    vecInsertOne_char(&c, '0', 0);

    for (size_t i = 0; i < c.length; i++) {
        printf("%c, ", c.data[i]);
    }
    printf("\n");

    Vec_char c1 = vecNew_char();
    vecPush_char(&c1, 'h');
    vecPush_char(&c1, 'e');
    vecPush_char(&c1, 'l');
    vecPush_char(&c1, 'l');

    vecInsertSome_char(&c, c1, 1);

    for (size_t i = 0; i < c.length; i++) {
        printf("%c, ", c.data[i]);
    }
    printf("\n");
}