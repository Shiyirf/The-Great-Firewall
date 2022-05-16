#pragma once

#include "node.h"
#include <stdbool.h>
#include <stdint.h>

extern uint64_t branches;

Node *bst_create(void);

uint32_t bst_height(Node *root);

uint32_t bst_size(Node *root);

Node *bst_find(Node *root, char *oldspeak);

Node *bst_insert(Node *root, char *oldspeak, char *newspeak);

void bst_print(Node *root);

void bst_delete(Node **root);
