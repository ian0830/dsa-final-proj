#define encodingSpace 52
#define upperBound 2147483647
#define upperLimit 41297760
//#define testData 15

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"

typedef struct nameNode {
    int checkSum;
    int index;
    char *name;
    struct nameNode *left;
    struct nameNode *right;
} NameNode;

int calculateCheckSum(char *string) {  // May occurs collision
    int checkSum = 0;
    int index = 0;
    while (string[index] != '\0') {
        checkSum = (checkSum * encodingSpace + (int)string[index]) % upperLimit;
        //fprintf(stderr, "==checkSum %d\n", checkSum);
        index++;
    }
    return checkSum;
    // Ensure overflow won't occur
}

NameNode *insertBST(NameNode *nameBST, char *string, int id) {
    // With dummy node, real head stored at dummy.right
    // Only store specific names in the BST
    //fprintf(stderr, "::::::::::::::insertBST::::::::::::\n");
    NameNode *ptr = nameBST;
    NameNode *nameNode = malloc(sizeof(NameNode));
    nameNode->checkSum = calculateCheckSum(string);
    nameNode->index = id;
    nameNode->name = string;
    nameNode->left = NULL;
    nameNode->right = NULL;
    //fprintf(stderr, "=======insertBST %s %d======\n", string, nameNode->checkSum);
    while (true) {
        if (nameNode->checkSum < ptr->checkSum) {
            if (ptr->left == NULL) {
                ptr->left = nameNode;
                //fprintf(stderr, "insert left: %s %d\n", ptr->name, ptr->checkSum);
                return nameBST;
            } else {
                ptr = ptr->left;
            }
        } else {
            // Case of collision
            if ((nameNode->checkSum == ptr->checkSum) && (ptr->name != NULL) && (strcmp(nameNode->name, ptr->name) == 0)){
                //fprintf(stderr, "=======collision name %s======\n", string);
                return nameBST;
            }
            if (ptr->right == NULL) {
                ptr->right = nameNode;
                //fprintf(stderr, "insert right: %s %d\n", ptr->name, ptr->checkSum);
                return nameBST;
            } else {
                ptr = ptr->right;
            }
        }
    }
    // Insert into BST
}

NameNode *searchBST(NameNode *nameBST, char *string) {
    //fprintf(stderr, "::::::::::::::searchBST::::::::::::\n");
    int checkSum = calculateCheckSum(string);
    NameNode *nameNode = nameBST->right;  // real root

    while (nameNode != NULL) {
        if (checkSum < nameNode->checkSum) {
            if (nameNode->left == NULL) {
                return false;
            } else {
                nameNode = nameNode->left;
            }
        } else {
            if ((checkSum == nameNode->checkSum) && (strcmp(string, nameNode->name) == 0)) {
                return nameNode;
            }
            if (nameNode->right == NULL) {
                return false;
            } else {
                nameNode = nameNode->right;
            }
        }
    }
    // find the node of the string from BST
}

void inorderBST(NameNode *nameBST, char **array) {
    if (nameBST == NULL) return;
    if (nameBST->name == NULL)
        return inorderBST(nameBST->right, array);  // for dummy node
    inorderBST(nameBST->left, array);
    array[nameBST->index] = nameBST->name;
    //fprintf(stderr, "inorderBST node: %s %d %d\n", array[nameBST->index], nameBST->index, nameBST->checkSum);
    inorderBST(nameBST->right, array);
}

void inorderTraversalBST(NameNode *nameBST) {
    if (nameBST == NULL) return;
    if (nameBST->name == NULL)
        return inorderTraversalBST(nameBST->right);
    inorderTraversalBST(nameBST->left);
    fprintf(stderr, "nameBST node: %s %d %d\n", nameBST->name, nameBST->checkSum, nameBST->index);
    inorderTraversalBST(nameBST->right);
}

NameNode *generateNameBST(mail *mails, int len, int *mids) {
    NameNode *nameBST = malloc(sizeof(NameNode) * len);
    nameBST->checkSum = nameBST->index = -1;
    nameBST->name = nameBST->left = nameBST->right = NULL;
    for (int id = 0; id < len; id++) {
        mail *m = mails + mids[id];
        nameBST = insertBST(nameBST, m->from, id * 2);
        nameBST = insertBST(nameBST, m->to, id * 2 + 1);
        //fprintf(stderr, "insert %s %s\n", m->from, m->to);
    }
    //inorderTraversalBST(nameBST);
    return nameBST;
}