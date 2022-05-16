#include "bv.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

BitVector *bv_create(uint32_t length) {
    uint32_t l = length / 8;
    if (length % 8 != 0) {
        l++;
    }
    uint8_t *p = (uint8_t *) malloc(l);
    if (p == NULL) {
        return NULL;
    }
    memset(p, 0, l);
    BitVector *bv = (BitVector *) malloc(sizeof(BitVector));
    bv->length = length;
    bv->vector = p;
    return bv;
}

void bv_delete(BitVector **bv) {
    free((*bv)->vector);
    free(*bv);
    *bv = NULL;
}

uint32_t bv_length(BitVector *bv) {
    return bv->length;
}

bool bv_set_bit(BitVector *bv, uint32_t i) { //i should be 0 to length-1
    if (i < 0 || i >= bv->length) {
        return false;
    }
    uint32_t bytes, bits;
    bytes = i / 8;
    bits = i - bytes * 8;
    uint8_t tmp = 1;
    bv->vector[bytes] = bv->vector[bytes] | (tmp << (7 - bits));
    return true;
}

bool bv_clr_bit(BitVector *bv, uint32_t i) { //i should be 0 to length-1
    if (i < 0 || i >= bv->length) {
        return false;
    }
    uint32_t bytes, bits;
    bytes = i / 8;
    bits = i - bytes * 8;
    uint8_t tmp = 1;
    bv->vector[bytes] = bv->vector[bytes] & ~(tmp << (7 - bits));
    return true;
}

bool bv_get_bit(BitVector *bv, uint32_t i) { //i should be 0 to length-1
    if (i < 0 || i >= bv->length) {
        return false;
    }
    uint32_t bytes, bits;
    bytes = i / 8;
    bits = i - bytes * 8;
    uint8_t tmp = bv->vector[bytes];
    tmp = (tmp >> (7 - bits)) & 1;
    if (tmp) {
        return true;
    } else {
        return false;
    }
}

void bv_print(BitVector *bv) {
    uint32_t bytes, bits;
    for (uint32_t i = 0; i < bv->length; i++) {
        bytes = i / 8;
        bits = i - bytes * 8;
        printf("%d", (bv->vector[bytes] >> (7 - bits)) & 1);
    }
    printf("\n");
}
