
struct substring {
    char* pointer;
    int length;
    int checksum;
};

typedef struct word {
    enum {
        SYMBOL,
        TOKEN,
        END,
    } type;
    union {
        char symbol;
        struct substring token;
    } data;
} Word;

int priority(char c) {
    switch (c) {
        case '(':
        case ')':
            return 3;
        case '!':
            return 2;
        case '&':
            return 1;
        case '|':
            return 0;
        default:
            return -1;
    }
}
Word readWord(char** string) {
    Word w;
    if (priority(**string) == -1) {
        w.type = TOKEN;
        w.data.token.pointer = *string;
        while (priority(**string) == -1) (*string)++;
        w.data.token.length = (*string) - w.data.token.pointer;
        w.data.token.checksum = calculateChecksum(w.data.token.pointer, w.data.token.length);
    } else {
        w.type = SYMBOL;
        w.data.symbol = **string;
        (*string) += 1;
    }
    return w;
}

Word* infixToPostfix(char* expression) {
    Word* result = malloc(sizeof(Word) * 2049);
    int resultIndex = 0, stackIndex = -1;  //stackIndex 存目前已使用的最右格
    Word symbolStack[2048];
    // struct substring tokenStack[1024];
    while (*expression != '\0') {
        Word w = readWord(&expression);
        if (w.type == TOKEN) {
            result[resultIndex++] = w;
        }
        if (w.type == SYMBOL) {
            if (w.data.symbol == '(') {
                symbolStack[++stackIndex] = w;
            } else if (w.data.symbol == ')') {
                while (stackIndex >= 0 && symbolStack[stackIndex].data.symbol != '(') {
                    result[resultIndex++] = symbolStack[stackIndex];
                    stackIndex -= 1;
                }
                stackIndex -= 1;
            } else {
                while (stackIndex >= 0 && symbolStack[stackIndex].data.symbol != '(' && priority(symbolStack[stackIndex].data.symbol) >= priority(w.data.symbol)) {
                    result[resultIndex++] = symbolStack[stackIndex];
                    stackIndex -= 1;
                }
                symbolStack[++stackIndex] = w;
            }
        }
    }
    while (stackIndex >= 0)
        result[resultIndex++] = symbolStack[stackIndex--];
    result[resultIndex++].type = END;
    return result;
}

void printWords(Word* w) {
    while (w->type != END) {
        if (w->type == SYMBOL) {
            fprintf(stderr, "%c ", w->data.symbol);
        }
        if (w->type == TOKEN) {
            fprintf(stderr, "%.*s ", w->data.token.length, w->data.token.pointer);
        }
        w++;
    }
    fprintf(stderr, "\n");
}

// struct expr {
//     bool eval;
//     MidLinkedListNode * token;
//     bool result;
// };

void printMids(MidLinkedListNode* n) {
    while (n != NULL) {
        fprintf(stderr, "%d ", n->mid);
        n = n->next;
    }
    fprintf(stderr, "\n");
}
int calcAndReturnDelta(char c, MidLinkedListNode** tokenStack, MidLinkedListNode* nodePool, int* nodePoolIndex, int n_mails, int lastIndex) {
    if (c == '!') {
        // fprintf(stderr, "!");
        MidLinkedListNode* node = tokenStack[lastIndex];
        // printMids(node);

        // fprintf(stderr, "%p", node);
        MidLinkedListNode* head = NULL;
        MidLinkedListNode* tail = NULL;
        for (int i = 0; i < n_mails; i++) {
            if (node == NULL) {
                MidLinkedListNode* n = nodePool + ((*nodePoolIndex)++);
                n->mid = i;
                n->next = NULL;
                if (tail != NULL) {
                    tail->next = n;
                    tail = n;
                } else {
                    head = n;
                    tail = n;
                }
            } else {
                if (i < (node->mid)) {
                    MidLinkedListNode* n = nodePool + ((*nodePoolIndex)++);
                    n->mid = i;
                    if (tail != NULL) {
                        tail->next = n;
                        tail = n;
                    } else {
                        head = n;
                        tail = n;
                    }
                } else {
                    node = node->next;
                }
            }
        }
        if (tail) tail->next = NULL;
        tokenStack[lastIndex] = head;
        // printMids(head);
        return 0;
    }
    if (c == '&') {
        // fprintf(stderr, "&");
        // fprintf(stderr, "lastIndex = %d,", lastIndex);
        MidLinkedListNode* nodeA = tokenStack[lastIndex];
        MidLinkedListNode* nodeB = tokenStack[lastIndex - 1];
        if (nodeA == NULL || nodeB == NULL) {
            tokenStack[lastIndex - 1] = NULL;
            return -1;
        }
        MidLinkedListNode* head = NULL;
        MidLinkedListNode* tail = NULL;
        for (int i = 0; i < n_mails; i++) {
            if ((nodeA && i == nodeA->mid) && (nodeB && i == nodeB->mid)) {
                MidLinkedListNode* n = nodePool + ((*nodePoolIndex)++);
                n->mid = i;
                n->next = NULL;
                if (tail != NULL) {
                    tail->next = n;
                    tail = n;
                } else {
                    head = n;
                    tail = n;
                }
            }
            if (nodeA && nodeA->mid <= i) {
                nodeA = nodeA->next;
            }
            if (nodeB && nodeB->mid <= i) {
                nodeB = nodeB->next;
            }
        }
        if (tail) tail->next = NULL;
        tokenStack[lastIndex - 1] = head;
        return -1;
    }
    if (c == '|') {
        // fprintf(stderr, "|");
        MidLinkedListNode* nodeA = tokenStack[lastIndex];
        MidLinkedListNode* nodeB = tokenStack[lastIndex - 1];
        if (nodeA == NULL) {
            tokenStack[lastIndex - 1] = nodeB;
            return -1;
        }
        if (nodeB == NULL) {
            tokenStack[lastIndex - 1] = nodeA;
            return -1;
        }
        MidLinkedListNode* head = NULL;
        MidLinkedListNode* tail = NULL;
        for (int i = 0; i < n_mails; i++) {
            if ((nodeA && i == nodeA->mid) || (nodeB && i == nodeB->mid)) {
                MidLinkedListNode* n = nodePool + ((*nodePoolIndex)++);
                n->mid = i;
                n->next = NULL;
                if (tail != NULL) {
                    tail->next = n;
                    tail = n;
                } else {
                    head = n;
                    tail = n;
                }
            }
            if (nodeA && nodeA->mid <= i) {
                nodeA = nodeA->next;
            }
            if (nodeB && nodeB->mid <= i) {
                nodeB = nodeB->next;
            }
        }
        // fprintf(stderr, "endfor");
        if (tail) tail->next = NULL;
        tokenStack[lastIndex - 1] = head;
        return -1;
    }
}


MidLinkedListNode* calculatePostfix(TokenLinkedListNode** mailTokenHashTable, Word* postfix, MidLinkedListNode* nodePool, int n_mails) {
    MidLinkedListNode* tokenStack[1024];
    int nodePoolIndex = 0;

    int stackIndex = -1;
    // fprintf(stderr, "entered calculatePostfix\n");
    while (postfix->type != END) {
        if (postfix->type == TOKEN) {
            // fprintf(stderr, "%.*s",  postfix->data.token.length, postfix->data.token.pointer);
            // printMids(getTokenOccurenceMids(mailTokenHashTable, postfix->data.token.pointer, postfix->data.token.length, postfix->data.token.checksum));
            tokenStack[++stackIndex] = getTokenOccurenceMids(mailTokenHashTable, postfix->data.token.pointer, postfix->data.token.length, postfix->data.token.checksum);
        }
        if (postfix->type == SYMBOL) {
            stackIndex += calcAndReturnDelta(postfix->data.symbol, tokenStack, nodePool, &nodePoolIndex, n_mails, stackIndex);
            // printMids(tokenStack[stackIndex]);
        }
        postfix++;
    }
    //remember to persist tokenStack[st]
    return tokenStack[stackIndex];
}

void expressionMatch(char* expression, int n_mails, MidLinkedListNode* nodePool, int* answers, int* answerLength, TokenLinkedListNode** mailTokenOccurenceHashTable) {
    Word* postfix = infixToPostfix(expression);
    int index = 0;
    MidLinkedListNode* node = (calculatePostfix(mailTokenOccurenceHashTable, postfix, nodePool, n_mails));
    while (node != NULL) {
        answers[index++] = node->mid;
        node = node->next;
    }
    free(postfix);
    *answerLength = index;
    return;
}