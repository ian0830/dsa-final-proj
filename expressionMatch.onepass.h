
struct substring {
    char* pointer;
    int length;
    short checksum;
};

typedef struct word {
    enum {
        SYMBOL,
        TOKEN,
        END,
        BOOL
    } type;
    union {
        char symbol;
        struct substring token;
        bool value;
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

int calcAndReturnDelta(char c, bool* boolStack, int lastIndex) {
    if (c == '!') {
        bool a = boolStack[lastIndex];
        boolStack[lastIndex] = !a;
        return 0;
    }
    if (c == '&') {
        bool a = boolStack[lastIndex];
        bool b = boolStack[lastIndex - 1];
        boolStack[lastIndex - 1] = a && b;
        return -1;
    }
    if (c == '|') {
        bool a = boolStack[lastIndex];
        bool b = boolStack[lastIndex - 1];
        boolStack[lastIndex - 1] = a || b;
        return -1;
    }
}

bool calculate(LinkedListNode** mailTokenHashTable, char* expression) {
    Word* result = malloc(sizeof(Word) * 2049);
    int resultIndex = 0, symbolStackIndex = -1;  //stackIndex 存目前已使用的最右格
    Word symbolStack[2048];
    bool boolStack[1024];
    int boolStackIndex = -1;
    while (*expression != '\0') {
        Word w = readWord(&expression);
        if (w.type == TOKEN) {
            boolStack[++boolStackIndex] = exist(mailTokenHashTable, w.data.token.pointer, w.data.token.length, w.data.token.checksum);
        }
        if (w.type == SYMBOL) {
            if (w.data.symbol == '(') {
                symbolStack[++symbolStackIndex] = w;
            } else if (w.data.symbol == ')') {
                while (symbolStackIndex >= 0 && symbolStack[symbolStackIndex].data.symbol != '(') {
                    boolStackIndex += calcAndReturnDelta(symbolStack[symbolStackIndex--].data.symbol, boolStack, boolStackIndex);
                }
                symbolStackIndex -= 1;
            } else {
                while (symbolStackIndex >= 0 && symbolStack[symbolStackIndex].data.symbol != '(' && priority(symbolStack[symbolStackIndex].data.symbol) >= priority(w.data.symbol)) {
                    boolStackIndex += calcAndReturnDelta(symbolStack[symbolStackIndex--].data.symbol, boolStack, boolStackIndex);
                }
                symbolStack[++symbolStackIndex] = w;
            }
        }
    }
    while (symbolStackIndex >= 0)
        boolStackIndex += calcAndReturnDelta(symbolStack[symbolStackIndex--].data.symbol, boolStack, boolStackIndex);
    fprintf(stderr, "boolStackIndex = %d",  boolStackIndex);
    return boolStack[boolStackIndex];
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

// bool calculatePostfix(LinkedListNode** mailTokenHashTable, Word* postfix, bool print) {
//     // fprintf(stderr, "entered calculatePostfix\n");
//     while (postfix->type != END) {
//         if (postfix->type == TOKEN) {
//             // if (print) {
//             //     printBST(tokenBST);
//             //     fprintf(stderr, "binarySearch(tokenBST, %.*s) = %d\n", postfix ->data.token.length, postfix -> data.token.pointer,binarySearch(tokenBST, postfix -> data.token.pointer, postfix ->data.token.length));
//             // }
//             boolStack[++boolStackIndex] = exist(mailTokenHashTable, postfix->data.token.pointer, postfix->data.token.length, postfix->data.token.checksum);
//         }
//         if (postfix->type == SYMBOL) {
//             if (postfix->data.symbol == '!') {
//                 bool a = boolStack[boolStackIndex--];
//                 boolStack[++boolStackIndex] = !a;
//             }
//             if (postfix->data.symbol == '&') {
//                 bool a = boolStack[boolStackIndex--];
//                 bool b = boolStack[boolStackIndex--];
//                 boolStack[++boolStackIndex] = a && b;
//             }
//             if (postfix->data.symbol == '|') {
//                 bool a = boolStack[boolStackIndex--];
//                 bool b = boolStack[boolStackIndex--];
//                 boolStack[++boolStackIndex] = a || b;
//             }
//         }
//         postfix++;
//     }
//     return boolStack[boolStackIndex];
// }

void expressionMatch(char* expression, int n_mails, int* answers, int* answerLength, LinkedListNode*** mailTokenHashTables) {
    // Word* postfix = infixToPostfix(expression);
    int index = 0;
    // fprintf(stderr, "infix converted to postfix\n");
    for (int i = 0; i < n_mails; i++) {
        if (calculate(mailTokenHashTables[i], expression)) {
            (answers)[index++] = i;
        }
    }
    *answerLength = index;
    return;
}