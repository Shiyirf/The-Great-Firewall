#include "bf.h"
#include "salts.h"
#include "speck.h"
#include <stdlib.h>

struct BloomFilter {
    uint64_t primary[2];
    uint64_t secondary[2];
    uint64_t tertiary[2];
    BitVector *filter;
};

BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));
    if (bf == NULL) {
        return NULL;
    }
    bf->filter = bv_create(size);
    if (bf->filter == NULL) {
        return NULL;
    }
    bf->primary[0] = SALT_PRIMARY_LO;
    bf->primary[1] = SALT_PRIMARY_HI;
    bf->secondary[0] = SALT_SECONDARY_LO;
    bf->secondary[1] = SALT_SECONDARY_HI;
    bf->tertiary[0] = SALT_TERTIARY_LO;
    bf->tertiary[1] = SALT_TERTIARY_HI;
    return bf;
}

void bf_delete(BloomFilter **bf) {
    BitVector **bvp = &((*bf)->filter);
    bv_delete(bvp);
    free(*bf);
    *bf = NULL;
}

uint32_t bf_size(BloomFilter *bf) {
    return bv_length(bf->filter);
}

void bf_insert(BloomFilter *bf, char *oldspeak) {
    uint32_t index[3];
    index[0] = hash(bf->primary, oldspeak);
    index[1] = hash(bf->secondary, oldspeak);
    index[2] = hash(bf->tertiary, oldspeak);
    uint32_t length = bf_size(bf);
    for (int i = 0; i < 3; i++) {
        bv_set_bit(bf->filter, index[i] % length); //the return value of bv_set_bit is unused.
    }
}

bool bf_probe(BloomFilter *bf, char *oldspeak) {
    uint32_t index[3];
    index[0] = hash(bf->primary, oldspeak);
    index[1] = hash(bf->secondary, oldspeak);
    index[2] = hash(bf->tertiary, oldspeak);
    uint32_t length = bf_size(bf);
    bool flag = true;
    for (int i = 0; i < 3; i++) {
        flag &= bv_get_bit(bf->filter, index[i] % length);
    }
    return flag;
}

uint32_t bf_count(BloomFilter *bf) {
    uint32_t length = bf_size(bf);
    uint32_t count = 0;
    for (uint32_t i = 0; i < length; i++) {
        if (bv_get_bit(bf->filter, i)) {
            count++;
        }
    }
    return count;
}

void bf_print(BloomFilter *bf) {
    bv_print(bf->filter);
}
