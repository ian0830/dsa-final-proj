
struct substring {
    char* pointer;
    int length;
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

bool calculatePostfix(LinkedListNode ** mailTokenHashTable, Word* postfix, bool print) {
    bool boolStack[1024];
    int stackIndex = -1;
    while (postfix -> type != END) {
        if (postfix -> type == TOKEN) {
            // if (print) {
            //     printBST(tokenBST);
            //     fprintf(stderr, "binarySearch(tokenBST, %.*s) = %d\n", postfix ->data.token.length, postfix -> data.token.pointer,binarySearch(tokenBST, postfix -> data.token.pointer, postfix ->data.token.length));
            // }
            boolStack[++stackIndex] = exist(mailTokenHashTable, postfix -> data.token.pointer, postfix ->data.token.length);
        }
        if (postfix -> type == SYMBOL) {
            if (postfix->data.symbol == '!') {
                bool a = boolStack[stackIndex--];
                boolStack[++stackIndex] = !a;
            }
            if (postfix->data.symbol == '&') {
                bool a = boolStack[stackIndex--];
                bool b = boolStack[stackIndex--];
                boolStack[++stackIndex] = a && b;
            }
            if (postfix->data.symbol == '|') {
                bool a = boolStack[stackIndex--];
                bool b = boolStack[stackIndex--];
                boolStack[++stackIndex] = a || b;
            }
        }
        postfix++;
    }
    return boolStack[stackIndex];
}

void expressionMatch(char* expression, int n_mails, int* answers, int* answerLength, LinkedListNode *** mailTokenHashTables) {
    Word* postfix = infixToPostfix(expression);
    int index = 0;
    for (int i = 0; i < n_mails; i++) {
        if (calculatePostfix(mailTokenHashTables[i], postfix, i == 1736 || i == 4681)) {
            (answers)[index++] = i;
        }
    }
    *answerLength = index;
    return;
}