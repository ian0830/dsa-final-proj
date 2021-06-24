#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"

typedef struct node {
    long long checksum;
    char* token;
} Node;

long long calculateChecksum(char* string) {
    // remember not to overflow
}

void insert(Node* BST, char* string) {
    int checksum = calculateChecksum(string);
    // insert into BST
}

bool isToken() {
    // determine whether A-Za-z0-9
}

Node** generateTokenBSTs(mail* mails, int length) {
    Node** tokenBSTs = malloc(sizeof(Node*) * length);
    for (int i = 0; i < length; i++) {
        Node* tokenBST = tokenBSTs[i];
        // parse token from each mail
    }
    return tokenBSTs;
}