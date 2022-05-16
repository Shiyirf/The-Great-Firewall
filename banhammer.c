#include "parser.h"
#include "bf.h"
#include "ht.h"
#include "messages.h"
#include <unistd.h>
#include <stdlib.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define WORD "[0-9a-zA-Z_]+(('|-)[0-9a-zA-Z_]+)*"

typedef struct linknode linknode;
struct linknode {
    char *oldspeak;
    char *newspeak;
    linknode *next;
};

linknode *link_create(char *oldspeak, char *newspeak) {
    linknode *l = (linknode *) malloc(sizeof(linknode));
    l->next = NULL;
    l->oldspeak = strdup(oldspeak);
    if (newspeak != NULL) {
        l->newspeak = strdup(newspeak);
    } else {
        l->newspeak = NULL;
    }
    return l;
}

linknode *link_insert(linknode *head, char *oldspeak, char *newspeak) {
    if (head == NULL) {
        head = link_create(oldspeak, newspeak);
        return head;
    }
    if (strcmp(head->oldspeak, oldspeak) > 0) {
        linknode *tmp = head;
        head = link_create(oldspeak, newspeak);
        head->next = tmp;
        return head;
    } else if (strcmp(head->oldspeak, oldspeak) < 0) {
        head->next = link_insert(head->next, oldspeak, newspeak);
    }
    return head;
}

void link_delete(linknode **head) {
    if (*head == NULL) {
        return;
    }
    link_delete(&((*head)->next));
    free((*head)->oldspeak);
    if ((*head)->newspeak != NULL) {
        free((*head)->newspeak);
    }
    free(*head);
    *head = NULL;
}

void link_print(linknode *head) {
    if (head == NULL) {
        return;
    }
    if (head->newspeak != NULL) {
        printf("%s -> %s\n", head->oldspeak, head->newspeak);
    } else {
        printf("%s\n", head->oldspeak);
    }
    link_print(head->next);
}

int main(int argc, char *argv[]) {
    int ch;
    uint32_t htsize = 1 << 16;
    uint32_t bfsize = 1 << 20;
    bool verbose = false;

    while ((ch = getopt(argc, argv, "t:f:sh")) != -1) {
        switch (ch) {
        case 't': sscanf(optarg, "%u", &htsize); break;

        case 'f': sscanf(optarg, "%u", &bfsize); break;

        case 's': verbose = true; break;

        default: // show help
            printf("SYNOPSIS\n\tA word filtering program for the GPRSC.\n\tFilters out and reports "
                   "bad words parsed from stdin.\n\n");
            printf("USAGE\n\t%s [-hs] [-t size] [-f size]\n\n", argv[0]);
            printf("OPTIONS\n");
            printf("\t-h              Program usage and help.\n");
            printf("\t-s              Print program statistics\n");
            printf("\t-t size         Specify hash table size (default: 2^16)\n");
            printf("\t-f size         Specify Bloom filter size (default: 2^20).\n");
            exit(0);
        }
    }

    // Initiallize bloom filter and hash table
    BloomFilter *bf = bf_create(bfsize);
    HashTable *ht = ht_create(htsize);
    char oldspeak[30], newspeak[30];
    linknode *badlink = NULL, *oldlink = NULL;

    FILE *badfp = fopen("badspeak.txt", "r");
    if (badfp == NULL) {
        fprintf(stderr, "%s open failed!\n", "badspeak.txt");
        exit(1);
    }
    while (fscanf(badfp, "%s", oldspeak) != EOF) {
        bf_insert(bf, oldspeak);
        ht_insert(ht, oldspeak, NULL);
    }

    FILE *newfp = fopen("newspeak.txt", "r");
    if (newfp == NULL) {
        fprintf(stderr, "%s open failed!\n", "newspeak.txt");
        exit(1);
    }
    while (fscanf(newfp, "%s %s", oldspeak, newspeak) != EOF) {
        bf_insert(bf, oldspeak);
        ht_insert(ht, oldspeak, newspeak);
    }

    regex_t re;
    if (regcomp(&re, WORD, REG_EXTENDED)) {
        fprintf(stderr, "Failed to compile regex.\n");
        return 1;
    }

    char *word = NULL;
    while ((word = next_word(stdin, &re)) != NULL) {
        for (int i = 0; word[i]; i++) { // transform to lowercase
            word[i] = tolower(word[i]);
        }
        //printf("%s\n",word);
        if (bf_probe(bf, word)) { //oldspeak maybe
            Node *n = ht_lookup(ht, word);
            if (n != NULL) {
                if (n->newspeak != NULL) { // oldspeak
                    oldlink = link_insert(oldlink, n->oldspeak, n->newspeak);
                } else { // badspeak
                    badlink = link_insert(badlink, n->oldspeak, NULL);
                }
            }
        }
    }

    if (verbose) {
        printf("Average BST size: %.6lf\n", ht_avg_bst_size(ht));
        printf("Average BST height: %.6lf\n", ht_avg_bst_height(ht));
        printf("Average branches traversed: %.6lf\n", (branches - lookups) / (double) lookups);
        printf("Hash table load: %.6lf%%\n", 100 * ht_count(ht) / (double) ht_size(ht));
        printf("Bloom filter load: %.6lf%%\n", 100 * bf_count(bf) / (double) bf_size(bf));
    } else {
        if (badlink != NULL && oldlink != NULL) {
            printf("%s", mixspeak_message);
            link_print(badlink);
            link_print(oldlink);
        } else if (badlink != NULL) {
            printf("%s", badspeak_message);
            link_print(badlink);
        } else if (oldlink != NULL) {
            printf("%s", goodspeak_message);
            link_print(oldlink);
        }
    }
    link_delete(&oldlink);
    link_delete(&badlink);
    clear_words();
    regfree(&re);
    bf_delete(&bf);
    ht_delete(&ht);
    fclose(badfp);
    fclose(newfp);
}
