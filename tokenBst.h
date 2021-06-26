#define contentSize 100000
#define encodingSpace 36
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

int charIndex(char c) {
    if (c >= 48 && c <= 57) {
        return (int)c - 48;
    }
    if (c >= 65 && c <= 90) {
        return 10 + (int)c - 65;
    }
    if (c >= 97 && c <= 122) {
        return 10 + (int)c - 97;
    }
}

unsigned long long calculateChecksum(char* string, int length) {
    unsigned long long checksum = 0;
    for (int index = 0; index < length; index++) {
        checksum = (checksum * encodingSpace + charIndex(string[index])) % upperBound;
    }
    return checksum;
    // remember not to overflow
}

void insert(Node* BST, char* string, int length) {
    // 某子樹的左邊必嚴格小於某子樹根
    // 某子樹的右邊必大於等於某子樹根
    // 如果token 已經存在於BST就不加入了
    unsigned long long checksum = calculateChecksum(string, length);
    Node* node = malloc(sizeof(Node));
    Node* pointer = BST;
    node->checksum = checksum;
    node->token = string;
    node->left = NULL;
    node->right = NULL;
    while (true) {
        if (checksum < pointer->checksum) {
            if (pointer->left == NULL) {
                pointer->left = node;
                break;
            } else
                pointer = pointer->left;
        } else {
            if (pointer->checksum == checksum && pointer->token != NULL && strcmp(pointer->token, string) == 0) return;
            if (pointer->right == NULL) {
                pointer->right = node;
                break;
            } else
                pointer = pointer->right;
        }
    }
    // insert into BST
}

bool caseInsensitiveCmp(char* a, char* b, int length) {
    bool same = true;
    for (int i = 0; i < length; i++) {
        if (charIndex(a[i]) != charIndex(b[i])) {
            same = false;
            break;
        }
    }
    return same;
}

bool binarySearch(Node* tokenBST, char* string, int length) {
    // 某子樹的左邊必嚴格小於某子樹根
    // 某子樹的右邊必大於等於某子樹根
    int checksum = calculateChecksum(string, length);
    Node* node = tokenBST->right;
    while (node != NULL) {
        if (checksum < node->checksum) {
            if (node->left == NULL) {
                return false;
            } else {
                node = node->left;
            }
        } else {
            if (checksum == node->checksum && strlen(node->token) == length && caseInsensitiveCmp(string, node->token, length)) return true;
            if (node->right == NULL) {
                return false;
            } else {
                node = node->right;
            }
        }
    }
}
void printBST(Node* node) {
    if (node == NULL) {
        return;
    }
    printBST(node->left);
    fprintf(stderr, "%s(%d) ", node->token, node->checksum);
    printBST(node->right);
}

bool isValid(char c) {
    // determine whether A-Za-z0-9
    return (c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
}

Node** generateTokenBSTs(mail* mails, int length) {
    Node** tokenBSTs = malloc(sizeof(Node*) * length);
    for (int i = 0; i < length; i++) {
        Node* tokenBST = malloc(sizeof(Node));
        tokenBST->checksum = 0;  // dummy node 的 checksum 是 0
        tokenBST->token = tokenBST->left = tokenBST->right = NULL;
        mail m = mails[i];
        char* token = malloc(sizeof(char) * (contentSize * 2));
        int tokenStartIndex = 0, tokenCurrentIndex = 0, contentIndex = 0;
        while (m.content[contentIndex] != '\0') {
            if (isValid(m.content[contentIndex])) {
                token[tokenCurrentIndex++] = m.content[contentIndex];
            } else if (tokenCurrentIndex != tokenStartIndex) {
                token[tokenCurrentIndex++] = '\0';
                insert(tokenBST, token + tokenStartIndex, tokenCurrentIndex - tokenStartIndex - 1);
                tokenStartIndex = tokenCurrentIndex;
            }
            contentIndex++;
        }
        tokenBSTs[i] = tokenBST;
    }
    return tokenBSTs;
}