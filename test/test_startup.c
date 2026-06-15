#include <stdio.h>
#include <string.h>

/* simulate .data copy */
void test_data_copy() {
    unsigned int src[] = {1, 2, 3, 4};
    unsigned int dst[4] = {0};
    unsigned int *s = src;
    unsigned int *d = dst;
    unsigned int *end = dst + 4;

    /* same logic as your startup */
    if (d != end) {
        while (d != end) {
            *d = *s;
            d++; s++;
        }
    }

    /* verify */
    for (int i = 0; i < 4; i++) {
        if (dst[i] != src[i]) {
            printf("FAIL: dst[%d] = %d, expected %d\n", i, dst[i], src[i]);
            return;
        }
    }
    printf("PASS: data copy works\n");
}

/* simulate .bss zero */
void test_bss_zero() {
    unsigned int bss[4] = {0xDEAD, 0xBEEF, 0xCAFE, 0xBABE};
    unsigned int *d = bss;
    unsigned int *end = bss + 4;

    if (d != end) {
        while (d != end) {
            *d = 0;
            d++;
        }
    }

    for (int i = 0; i < 4; i++) {
        if (bss[i] != 0) {
            printf("FAIL: bss[%d] = %d, expected 0\n", i, bss[i]);
            return;
        }
    }
    printf("PASS: bss zero works\n");
}

int main() {
    test_data_copy();
    test_bss_zero();
    return 0;
}