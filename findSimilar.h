typedef struct similarBST {
    double Similar;
    int position;
    struct similarBST* left;
    struct similarBST* right;
} SimilarBST;

void Insert(SimilarBST* Root, double Similar, int position) {
    SimilarBST* node = malloc(sizeof(SimilarBST));
    SimilarBST* pointer = Root;
    node->Similar = Similar;
    node->position = position;
    node->left = NULL;
    node->right = NULL;
    while (true) {
        if (pointer->Similar >= Similar) {
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
}

int Size(Node* node, int number) {
    if (node->left == NULL && node->right == NULL)
        return 1;
    else if (node->left != NULL && node->right != NULL)
        return 1 + Size(node->left, 0) + Size(node->right, 0);
    else if (node->left != NULL)
        return 1 + Size(node->left, 0);
    else
        return 1 + Size(node->right, 0);
}
int idx = 0;
void GiveChecksum(unsigned long long** Purpose, Node* node) {
    (*Purpose)[idx] = node->checksum;
    if (node->left != NULL) {
        GiveChecksum(&(*Purpose), node->left);
    }
    if (node->right != NULL) {
        GiveChecksum(&(*Purpose), node->right);
    }
}

bool isToken(long long int hash, Node* mail) {
    if (hash == mail->checksum)
        return true;
    else if (mail->left != NULL || mail->right != NULL) {
        if (mail->left == NULL)
            return isToken(hash, mail->right);
        else if (mail->right == NULL)
            return isToken(hash, mail->left);
        else
            return (isToken(hash, mail->left) || isToken(hash, mail->right));
    } else
        return false;
}

double CompareChecksum(unsigned long long* Purpose, Node* mail, int Length) {
    // Purpose is Token's hashs, mail is the mail[i], Length is the size of
    bool K;
    int AND = 0;
    int mail_size = 0;
    double Similarity = 0;
    for (int i = 0; i < Length; i++) {
        K = isToken(Purpose[i], mail);
        if (K == true) AND++;
    }
    mail_size = Size(mail, 0);
    Similarity = AND / (Length + mail_size - AND);
    return Similarity;
};

void split(SimilarBST* S, int thresold, SimilarBST** a, SimilarBST** b) {
    if (S == NULL) {
        *a = *b = NULL;
        return;
    }
    if (S->left >= thresold) {
        *b = S;
        split(S->left, thresold, a, &((*b)->left));
    } else {
        *a = S;
        split(S->right, thresold - 1, &((*a)->right), b);
    }
}

int pos = 0;
void writeIn(SimilarBST* node, int** answers) {
    if (node->left !=NULL)
        writeIn(node->left, &answers);
    (*answers)[pos] = node->position;
    if (node->right != NULL)
        writeIn(node->right, &answers);
}

void findSimilar(int mid, double threshold, int** answers, int* answerLength,
                 Node** mailTokenBSTs) {
    // similar = (A ∩ B)/(A U B)
    // mailTokenBSTs = forest
    SimilarBST* Root = malloc(sizeof(SimilarBST));
    Node* PurposeMail = mailTokenBSTs[mid];
    double PurposeSimilar = 0;
    int TokenSize = Size(mailTokenBSTs[mid], 0);
    answerLength = 0;
    unsigned long long Purpose[TokenSize];
    GiveChecksum(&Purpose, PurposeMail);
    for (int i = 0; i < answerLength; i++) {  // mails[i]
        if (i == mid) {
            continue;
        }
        for (int j = 0; j < TokenSize; j++) {  // for evef
            PurposeSimilar =
                CompareChecksum(Purpose, mailTokenBSTs[i], TokenSize);
            Insert(Root, PurposeSimilar, i);
            if (PurposeSimilar >= threshold) {
                answerLength++;
            }
        }
    }
    SimilarBST* a = malloc(sizeof(SimilarBST));
    SimilarBST* b = malloc(sizeof(SimilarBST));
    split(Root, threshold, &a, &b);
    writeIn(Root, &answers);
}