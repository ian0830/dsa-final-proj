#define contentSize 100000
#define encodingSpace 36
#define hashTableSize 10000
#define upperBound hashTableSize

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct linkedListNode {
    char* token;
    int length;
    struct linkedListNode* next;
} LinkedListNode;

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

bool exist(char* hashTable, char* token, int length) {
    int checksum = calculateChecksum(token, length);
    if (hashTable[checksum] == NULL)
        return false;
    else {
        LinkedListNode* node = hashTable[checksum];
        bool existance = false;
        while (node != NULL) {
            if (caseInsensitiveCmp(node->token, token, length)) {
                existance = true;
                break;
            }
            node = node->next;
        }
        return existance;
    }
}

void insert(LinkedListNode** hashTable, char* token, int length) {
    LinkedListNode* node = malloc(sizeof(LinkedListNode));
    node->length = length;
    node->token = token;
    int checksum = calculateChecksum(token, length);
    node->next = hashTable[checksum];
    hashTable[checksum] = node;
}

void printHashTable(LinkedListNode** hashTable) {
    // struct {
    //     int checksum;
    //     LinkedListNode * node;
    // } collection[contentSize / 2 + 1];
    // int collectionIndex = 0;
    for (int i = 0; i < hashTableSize; i++) {
        if (hashTable[i] != NULL) {
            LinkedListNode* node = hashTable[i];
            while (node != NULL) {
                fprintf(stderr, "%.*s(%d) ", node->length, node->token, i);
                // collection[collectionIndex].checksum = i;
                // collection[collectionIndex].node = node;
                // collectionIndex ++;
                node = node->next;
            }
        }
    }
    // qsort(collection, collectionIndex, sizeof(collection[0]), collectionCmp);
}

bool isValid(char c) {
    // determine whether A-Za-z0-9
    return (c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
}

LinkedListNode*** generateTokenHashTables(mail* mails, int length) {
    LinkedListNode*** tokenHashTables = malloc(sizeof(LinkedListNode**) * length);
    LinkedListNode ** memoryAllocate = malloc(sizeof(LinkedListNode *) * hashTableSize * length);
    time_t start = time(NULL);
    for (int i = 0; i < length; i++) {
        LinkedListNode** tokenHashTable = memoryAllocate + hashTableSize * i;
        for (int j = 0; j< hashTableSize; j++) tokenHashTable[j] = NULL;
        mail m = mails[i];
        char* token = malloc(sizeof(char) * (contentSize * 2));
        int tokenStartIndex = 0, tokenCurrentIndex = 0, contentIndex = 0;
        while (m.content[contentIndex] != '\0') {
            if (isValid(m.content[contentIndex])) {
                token[tokenCurrentIndex++] = m.content[contentIndex];
            } else if (tokenCurrentIndex != tokenStartIndex) {
                token[tokenCurrentIndex++] = '\0';
                insert(tokenHashTable, token + tokenStartIndex, tokenCurrentIndex - tokenStartIndex - 1);
                tokenStartIndex = tokenCurrentIndex;
            }
            contentIndex++;
        }
        tokenHashTables[i] = tokenHashTable;
    }
    fprintf(stderr, "indexing token time consumed: %d\n", time(NULL) - start);
    return tokenHashTables;
}