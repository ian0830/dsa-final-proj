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
        checkSum = (checkSum * encodingSpace + (int)string[index]) % upperBound;
        index++;
    }
    return checkSum;
    // Ensure overflow won't occur
}

void insertBST(Node *BST, char *string, int id) {
    // With dummy node, real head stored at dummy.right
    // Only store specific names in the BST
    Node *pointer = BST;
    Node *node = malloc(sizeof(Node));
    node->checkSum = calculateCheckSum(string);
    node->index = id;
    node->name = string;
    node->left = NULL;
    node->right = NULL;
    while (true) {
        if (node->checkSum < pointer->checkSum) {
            if (pointer->left == NULL) {
                pointer->left = node;
                break;
            } else {
                pointer = pointer->left;
            }
        } else {
            // Case of collision
            if ((node->checkSum == pointer->checkSum) &&
                (pointer->name != NULL) &&
                (strcmp(node->name, pointer->name) == 0))
                return;
            if (pointer->right == NULL) {
                pointer->right = node;
                break;
            } else {
                pointer = pointer->right;
            }
        }
    }
    // Insert into BST
}

Node *searchBST(Node *BST, char *string) {
    int checkSum = calculateCheckSum(string);
    Node *node = BST->right;  // real root
    while (node != NULL) {
        if (checkSum < node->checkSum) {
            if (node->left == NULL) {
                return false;
            } else {
                node = node->left;
            }
        } else {
            if (checkSum == node->checkSum && strcpy(string, node->name) == 0) {
                return node;
            }
            if (node->right == NULL) {
                return false;
            } else {
                node = node->right;
            }
        }
    }
    //find the node of the string from BST
}

Node *generateNameBST(mail *mails, int len, int *mids) {
    Node *nameBST = malloc(sizeof(Node) * len);
    for (int id = 0; id < len; id++) {
        mail m = mails[mids[id]];
        nameBST->checkSum = 0;
        nameBST->index = -1;
        nameBST->name = NULL;
        nameBST->left = NULL;
        nameBST->right = NULL;
        insertBST(nameBST, m.from, id * 2);
        insertBST(nameBST, m.to, id * 2 + 1);
    }
    return nameBST;
}