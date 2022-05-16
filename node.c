#include "node.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

Node *node_create(char *oldspeak, char *newspeak) {
    Node *n = (Node *) malloc(sizeof(Node));
    n->left = n->right = NULL;
    n->oldspeak = strdup(oldspeak);
    if (newspeak != NULL) {
        n->newspeak = strdup(newspeak);
    } else {
        n->newspeak = NULL;
    }
    return n;
}

void node_delete(Node **n) {
    free((*n)->oldspeak);
    if ((*n)->newspeak != NULL) {
        free((*n)->newspeak);
    }
    free(*n);
    *n = NULL;
}

void node_print(Node *n) {
    if (n->newspeak != NULL) {
        printf("%s -> %s\n", n->oldspeak, n->newspeak);
    } else {
        printf("%s\n", n->oldspeak);
    }
}
