#define contentSize 100256
#define hashTableSize 20000000

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct midLinkedListNode {
    int mid;
    struct midLinkedListNode* next;
} MidLinkedListNode;

typedef struct tokenLinkedListNode {
    char* token;
    int length;
    struct tokenLinkedListNode* next;
    struct midLinkedListNode* head;
    struct midLinkedListNode* tail;
} TokenLinkedListNode;

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

int calculateChecksum(char* string, int length) {
    int checksum = 0;
    for (int index = 0; index < length; index++) {
        checksum = (checksum * 36 + charIndex(string[index])) % hashTableSize;
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

TokenLinkedListNode* findOrCreate(TokenLinkedListNode** hashTable, char* token, int length, int checksum) {
    TokenLinkedListNode* result = NULL;
    TokenLinkedListNode* next = hashTable[checksum];

    if (hashTable[checksum] != NULL) {
        TokenLinkedListNode* node = hashTable[checksum];
        while (node != NULL) {
            if (node->length == length && caseInsensitiveCmp(node->token, token, length)) {
                result = node;
                break;
            }
            node = node->next;
        }
    }
    if (result == NULL) {
        result = malloc(sizeof(TokenLinkedListNode));
        result->length = length;
        result->token = token;
        result->next = next;
        result->head = NULL;
        result->tail = NULL;
        hashTable[checksum] = result;
        // create
    }
    return result;
}

void insert(TokenLinkedListNode** hashTable, char* token, int length, int mid) {
    int checksum = calculateChecksum(token, length);
    TokenLinkedListNode* node = (TokenLinkedListNode*)findOrCreate(hashTable, token, length, checksum);
    MidLinkedListNode* head = node->head;
    MidLinkedListNode* tail = node->tail;
    // if(tail != NULL) {
    //     fprintf(stderr, " %d == %d = %d ", tail -> mid,mid, tail->mid == mid);
    // }
    if (tail != NULL && tail -> mid == mid) {
     
        return;
    }
    MidLinkedListNode* midNode = malloc(sizeof(MidLinkedListNode));
    midNode->mid = mid;
    midNode->next = NULL;
    if (tail != NULL) {
        tail->next = midNode;
        node->tail = midNode;
    } else {
        node->head = midNode;
        node->tail = midNode;
    }
}

void printMid(MidLinkedListNode* n) {
    while (n != NULL) {
        fprintf(stderr, "%d ", n->mid);
        n = n->next;
    }
    fprintf(stderr, "\n");
}
MidLinkedListNode * getTokenOccurenceMids(TokenLinkedListNode** hashTable, char * token, int length, int checksum) {
    
    // fprintf(stderr, "%d", checksum);printMid(hashTable[checksum]->head);
    return findOrCreate(hashTable, token, length, checksum) -> head;
}

void printHashTable(TokenLinkedListNode** hashTable) {
    // struct {
    //     int checksum;
    //     LinkedListNode * node;
    // } collection[contentSize / 2 + 1];
    // int collectionIndex = 0;
    for (int i = 0; i < hashTableSize; i++) {
        if (hashTable[i] != NULL) {
            TokenLinkedListNode* node = hashTable[i];
            while (node != NULL) {
                fprintf(stderr, "%.*s(%d) ", node->length, node->token, i);
                fprintf(stderr, ": ");
                if (i < 5) {MidLinkedListNode * n = node ->head;
                while(n != NULL) {
                    fprintf(stderr, "%d ", n -> mid);
                    n = n -> next;
                }
                fprintf(stderr, "\n");}
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

TokenLinkedListNode** generateTokenOccurenceHashTable(mail* mails, int length) {
    TokenLinkedListNode** tokenHashTable = malloc(sizeof(TokenLinkedListNode*) * hashTableSize);
    for (int j = 0; j < hashTableSize; j++) tokenHashTable[j] = NULL;
    for (int i = 0; i < length; i++) {
        mail m = mails[i];
        char* token = malloc(sizeof(char) * (contentSize * 2));
        int tokenStartIndex = 0, tokenCurrentIndex = 0, contentIndex = 0;
        while (m.content[contentIndex] != '\0') {
            if (isValid(m.content[contentIndex])) {
                token[tokenCurrentIndex++] = m.content[contentIndex];
            } else if (tokenCurrentIndex != tokenStartIndex) {
                token[tokenCurrentIndex++] = '\0';
                insert(tokenHashTable, token + tokenStartIndex, tokenCurrentIndex - tokenStartIndex - 1, i);
                tokenStartIndex = tokenCurrentIndex;
            }
            contentIndex++;
        }
        if (tokenCurrentIndex != tokenStartIndex) {
            token[tokenCurrentIndex++] = '\0';
            insert(tokenHashTable, token + tokenStartIndex, tokenCurrentIndex - tokenStartIndex - 1, i);
            tokenStartIndex = tokenCurrentIndex;
        }
        int subjectIndex = 0;
        while (m.subject[subjectIndex] != '\0') {
            if (isValid(m.subject[subjectIndex])) {
                token[tokenCurrentIndex++] = m.subject[subjectIndex];
            } else if (tokenCurrentIndex != tokenStartIndex) {
                token[tokenCurrentIndex++] = '\0';
                insert(tokenHashTable, token + tokenStartIndex, tokenCurrentIndex - tokenStartIndex - 1, i);
                tokenStartIndex = tokenCurrentIndex;
            }
            subjectIndex++;
        }
        if (tokenCurrentIndex != tokenStartIndex) {
            token[tokenCurrentIndex++] = '\0';
            insert(tokenHashTable, token + tokenStartIndex, tokenCurrentIndex - tokenStartIndex - 1, i);
            tokenStartIndex = tokenCurrentIndex;
        }
    }
    return tokenHashTable;
}