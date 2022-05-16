#include "bst.h"
#include <stdlib.h>
#include <string.h>

uint64_t branches = 0;

Node *bst_create(void) {
    return NULL;
}

uint32_t bst_height(Node *root) {
    if (root == NULL) {
        return 0;
    }
    uint32_t lheight = bst_height(root->left);
    uint32_t rheight = bst_height(root->right);
    uint32_t max = (lheight > rheight) ? lheight : rheight;
    return max + 1;
}

uint32_t bst_size(Node *root) {
    if (root == NULL) {
        return 0;
    }
    uint32_t lsize = bst_size(root->left);
    uint32_t rsize = bst_size(root->right);
    return lsize + rsize + 1;
}

Node *bst_find(Node *root, char *oldspeak) {
    branches++;
    if (root == NULL) { // empty tree
        return NULL;
    }
    if (strcmp(root->oldspeak, oldspeak) == 0) { // find as root
        return root;
    }
    if (strcmp(root->oldspeak, oldspeak) < 0) { // find in right subtree
        return bst_find(root->right, oldspeak);
    } else { // find in left subtree
        return bst_find(root->left, oldspeak);
    }
}

Node *bst_insert(Node *root, char *oldspeak, char *newspeak) {
    branches++;
    if (root == NULL) { // empty tree
        root = node_create(oldspeak, newspeak);
        return root;
    }
    if (strcmp(root->oldspeak, oldspeak) < 0) { // find in right subtree
        root->right = bst_insert(root->right, oldspeak, newspeak);
    } else if (strcmp(root->oldspeak, oldspeak) > 0) { // find in left subtree
        root->left = bst_insert(root->left, oldspeak, newspeak);
    }
    return root;
}

void bst_print(Node *root) {
    if (root == NULL) {
        return;
    }
    bst_print(root->left);
    node_print(root);
    bst_print(root->right);
}

void bst_delete(Node **root) {
    if (*root == NULL) { // empty tree
        return;
    }
    bst_delete(&((*root)->left)); // postorder
    bst_delete(&((*root)->right));
    node_delete(root);
}
