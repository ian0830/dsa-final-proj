
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

struct expr {
    bool eval;
    Word* token;
    bool result;
};
int calcAndReturnDelta(char c, struct expr* tokenStack, LinkedListNode** mailTokenHashTable, int lastIndex) {
    if (c == '!') {
        struct expr* a = tokenStack + lastIndex;
        if (a->eval == false) {
            a->eval = true;
            a->result = exist(mailTokenHashTable, a->token->data.token.pointer, a->token->data.token.length, a->token->data.token.checksum);
        }
        a->result = !(a->result);
        return 0;
    }
    if (c == '&') {
        fflush(stderr);
        struct expr* a = tokenStack + lastIndex;
        struct expr* b = tokenStack + (lastIndex - 1);
        if ((a->eval == true && a->result == false) || (b->eval == true && b->result == false)) {
            
            tokenStack[lastIndex - 1].eval = true;
            tokenStack[lastIndex - 1].result = false;
            return -1;
        } else {
            if (a->eval == false) {
                a->eval = true;
                a->result = exist(mailTokenHashTable, a->token->data.token.pointer, a->token->data.token.length, a->token->data.token.checksum);
            }
            if (a->result == false) {
                tokenStack[lastIndex - 1].eval = true;
                tokenStack[lastIndex - 1].result = false;
                return -1;
            }
            if (b->eval == false) {
                b->eval = true;
                b->result = exist(mailTokenHashTable, b->token->data.token.pointer, b->token->data.token.length, b->token->data.token.checksum);
            }
            tokenStack[lastIndex - 1].result = (b->result);
            return -1;
        }
    }
    if (c == '|') {
        struct expr* a = tokenStack + lastIndex;
        struct expr* b = tokenStack + (lastIndex - 1);
        if ((a->eval == true && a->result == true) || (b->eval == true && b->result == true)) {
            tokenStack[lastIndex - 1].eval = true;
            tokenStack[lastIndex - 1].result = true;
            return -1;
        } else {
            if (a->eval == false) {
                a->eval = true;
                a->result = exist(mailTokenHashTable, a->token->data.token.pointer, a->token->data.token.length, a->token->data.token.checksum);
            }
            if (a->result == true) {
                tokenStack[lastIndex - 1].eval = true;
                tokenStack[lastIndex - 1].result = true;
                return -1;
            }
            if (b->eval == false) {
                b->eval = true;
                b->result = exist(mailTokenHashTable, b->token->data.token.pointer, b->token->data.token.length, b->token->data.token.checksum);
            }
            tokenStack[lastIndex - 1].result = (b->result);
            return -1;
        }
    }
}

bool calculatePostfix(LinkedListNode** mailTokenHashTable, Word* postfix, bool print) {
    struct expr tokenStack[1024];
    int stackIndex = -1;
    // fprintf(stderr, "entered calculatePostfix\n");
    while (postfix->type != END) {
        if (postfix->type == TOKEN) {
            // if (print) {
            //     printBST(tokenBST);
            //     fprintf(stderr, "binarySearch(tokenBST, %.*s) = %d\n", postfix ->data.token.length, postfix -> data.token.pointer,binarySearch(tokenBST, postfix -> data.token.pointer, postfix ->data.token.length));
            // }
            tokenStack[++stackIndex].eval = false;
            tokenStack[stackIndex].token = postfix;
            // tokenStack[++stackIndex] = postfix;
        }
        if (postfix->type == SYMBOL) {
            stackIndex += calcAndReturnDelta(postfix->data.symbol, tokenStack, mailTokenHashTable, stackIndex);
            // if (postfix->data.symbol == '!') {
            //     bool a = boolStack[stackIndex--];
            //     boolStack[++stackIndex] = !a;
            // }
            // if (postfix->data.symbol == '&') {
            //     bool a = boolStack[stackIndex--];
            //     bool b = boolStack[stackIndex--];
            //     boolStack[++stackIndex] = a && b;
            // }
            // if (postfix->data.symbol == '|') {
            //     bool a = boolStack[stackIndex--];
            //     bool b = boolStack[stackIndex--];
            //     boolStack[++stackIndex] = a || b;
            // }
        }
        postfix++;
    }
    if (tokenStack[stackIndex].eval == false) {
        tokenStack[stackIndex].eval = true;
        tokenStack[stackIndex].result = exist(mailTokenHashTable, tokenStack[stackIndex].token->data.token.pointer, tokenStack[stackIndex].token->data.token.length, tokenStack[stackIndex].token->data.token.checksum);
    }
    return tokenStack[stackIndex].result;
}

void expressionMatch(char* expression, int n_mails, int* answers, int* answerLength, LinkedListNode*** mailTokenHashTables) {
    Word* postfix = infixToPostfix(expression);
    int index = 0;
    // fprintf(stderr, "infix converted to postfix\n");
    for (int i = 0; i < n_mails; i++) {
        if (calculatePostfix(mailTokenHashTables[i], postfix, i == 1736 || i == 4681)) {
            (answers)[index++] = i;
        }
    }
    free(postfix);
    *answerLength = index;
    return;
}