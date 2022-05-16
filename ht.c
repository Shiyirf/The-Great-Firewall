#include "ht.h"
#include "salts.h"
#include "speck.h"
#include <stdlib.h>
#include <stdio.h>

uint64_t lookups = 0;
struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    Node **trees;
};

HashTable *ht_create(uint32_t size) {
    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
    ht->salt[0] = SALT_HASHTABLE_LO;
    ht->salt[1] = SALT_HASHTABLE_HI;
    ht->size = size;
    ht->trees = (Node **) malloc(size * sizeof(Node *));
    for (uint32_t i = 0; i < size; i++) {
        ht->trees[i] = bst_create();
    }
    return ht;
}

void ht_delete(HashTable **ht) {
    for (uint32_t i = 0; i < (*ht)->size; i++) {
        bst_delete((*ht)->trees + i);
    }
    free((*ht)->trees);
    free(*ht);
    *ht = NULL;
}

uint32_t ht_size(HashTable *ht) {
    return ht->size;
}

Node *ht_lookup(HashTable *ht, char *oldspeak) {
    lookups++;
    uint32_t index = hash(ht->salt, oldspeak);
    return bst_find(ht->trees[index % ht_size(ht)], oldspeak);
}

void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    lookups++;
    uint32_t index = hash(ht->salt, oldspeak);
    index = index % ht_size(ht);
    ht->trees[index] = bst_insert(ht->trees[index], oldspeak, newspeak);
}

uint32_t ht_count(HashTable *ht) {
    uint32_t cnt = 0;
    for (uint32_t i = 0; i < ht->size; i++) {
        if (ht->trees[i] != NULL) {
            cnt++;
        }
    }
    return cnt;
}

double ht_avg_bst_size(HashTable *ht) {
    double sum = 0;
    for (uint32_t i = 0; i < ht->size; i++) {
        sum += bst_size(ht->trees[i]);
    }
    return sum / ht_count(ht);
}

double ht_avg_bst_height(HashTable *ht) {
    double sum = 0;
    for (uint32_t i = 0; i < ht->size; i++) {
        sum += bst_height(ht->trees[i]);
    }
    return sum / ht_count(ht);
}

void ht_print(HashTable *ht) {
    printf("Hash Table Size: %d\n", ht->size);
    for (uint32_t i = 0; i < ht->size; i++) {
        bst_print((ht->trees)[i]);
        printf("\n");
    }
}
