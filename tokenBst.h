#define contentSize 100000
#define encodingSpace 62
#define upperBound 2147483647

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"

typedef struct node {
    long long checksum;
    char* token;
    struct node* left;
    struct node* right;
} Node;

unsigned long long calculateChecksum(char* string) {
    int index = 0;
    unsigned long long checksum = 0;
    while (string[index] != '\0') {
        checksum = (checksum * encodingSpace + (int)string[index]) % upperBound;
        index++;
    }
    return checksum;
    // remember not to overflow
}

void insert(Node* BST, char* string) {
    // 某子樹的左邊必小於等於某子樹根
    // 某子樹的右邊必嚴格大於某子樹根
    unsigned long long checksum = calculateChecksum(string);
    Node* node = malloc(sizeof(Node));
    Node* pointer = BST;
    node->checksum = checksum;
    node->token = string;
    node->left = NULL;
    node->right = NULL;
    while (true) {
        if (pointer->checksum >= checksum) {
            if (pointer->left == NULL) {
                pointer->left = node;
                break;
            } else
                pointer = pointer->left;
        } else {
            if (pointer->right == NULL) {
                pointer->right = node;
                break;
            } else
                pointer = pointer->right;
        }
    }
    // insert into BST
}

bool isValid(char c) {
    // determine whether A-Za-z0-9
    return (c >= 48 && c <= 57) || (c >= 65 && c <= 90) ||
           (c >= 97 && c <= 122);
}

Node** generateTokenBSTs(mail* mails, int length) {
    Node** tokenBSTs = malloc(sizeof(Node*) * length);
    for (int i = 0; i < length; i++) {
        Node* tokenBST = malloc(sizeof(Node));
        mail m = mails[i];
        char * token = malloc(sizeof(char) * (contentSize * 2));
        int tokenStartIndex = 0, tokenCurrentIndex = 0, contentIndex = 0;
        while (m.content[contentIndex] != '\0') {
            if (isValid(m.content[contentIndex])) {
                token[tokenCurrentIndex++] = m.content[contentIndex];
            } else if (tokenCurrentIndex != tokenStartIndex) {
                token[tokenCurrentIndex++] = '\0';
                insert(tokenBST, token + tokenStartIndex);
                tokenStartIndex = tokenCurrentIndex;
            }
            contentIndex++;
        }
        tokenBSTs[i] = tokenBST;
    }
    return tokenBSTs;
}