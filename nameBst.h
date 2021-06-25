#define encodingSpace 52
#define upperBound 2147483647

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"

typedef struct node {
    int checkSum;
    int index;
    char *name;
    struct node *left;
    struct node *right;
} Node;

int calculateCheckSum(char *string) {  // May occurs collision
    int checkSum = 0;
    int index = 0;
    while (string[index] != '\0') {
        checkSum = (checkSum * 52 + (int)string[index]) % upperBound;
        index++;
    }
    return checkSum;
}

void insertBST(Node *BST, char *string, int id) {  // with dummy node
    Node *pointer = BST;
    Node *node = malloc(sizeof(Node));
    node->checkSum = calculateCheckSum(string);
    node->index = id;
    node->name = string;
    node->left = NULL;
    node->right = NULL;
    // should be fixed below
    while (true) {
        if (pointer->checkSum >= node->checkSum) {
            if (pointer->left == NULL) {
                pointer->left == node;
                break;
            } else {
                pointer = pointer->left;
            }
        } else {
            if (pointer->right == NULL) {
                pointer->right = node;
                break;
            } else {
                pointer = pointer->right;
            }
        }
        // insert into BST
    }
}

Node *generateNameBST(mail *mails, int length, int len, int *mids) {
    Node *nameBST = malloc(sizeof(Node) * len);
    for (int id = 0; id < len; id++) {
        mail m = mails[mids[id]];
        nameBST->checkSum = 0;
        nameBST->index = -1;
        nameBST->name = "NA";
        nameBST->left = NULL;
        nameBST->right = NULL;
        insertBST(nameBST, m.from, id * 2);
        insertBST(nameBST, m.to, id * 2 + 1);
    }
    return nameBST;
}