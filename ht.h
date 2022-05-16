#pragma once

#include "bst.h"

#include <stdint.h>

extern uint64_t lookups;

typedef struct HashTable HashTable;

HashTable *ht_create(uint32_t size);

void ht_delete(HashTable **ht);

uint32_t ht_size(HashTable *ht);

Node *ht_lookup(HashTable *ht, char *oldspeak);

void ht_insert(HashTable *ht, char *oldspeak, char *newspeak);

uint32_t ht_count(HashTable *ht);

double ht_avg_bst_size(HashTable *ht);

double ht_avg_bst_height(HashTable *ht);

void ht_print(HashTable *ht);
