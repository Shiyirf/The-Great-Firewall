#pragma once

typedef struct Node Node;

struct Node {
    char *oldspeak;
    char *newspeak;
    Node *left;
    Node *right;
};

Node *node_create(char *oldspeak, char *newspeak);

void node_delete(Node **n);

void node_print(Node *n);
