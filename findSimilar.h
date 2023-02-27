typedef struct similarBST {
    double Similar;
    int position;
    struct similarBST* left;
    struct similarBST* right;
} SimilarBST;
typedef struct unsign {
    unsigned long long hash;
    char* string;
} Unsigned;

int Size(Node* node) {
    if (node->left == NULL && node->right == NULL)
        return 1;
    else if (node->left != NULL && node->right != NULL)
        return 1 + Size(node->left) + Size(node->right);
    else if (node->left != NULL)
        return 1 + Size(node->left);
    else
        return 1 + Size(node->right);
}
int Index = 0;
void GiveChecksum(Unsigned* Purpose, Node* node, int k) {
    if ( k == 0 ) Index = 0;
    // fprintf(stderr, "checksum = %d\n", node->checksum);
    // fflush(stderr);
    // fprintf(stderr, "Index = %d\n", Index);
    // fflush(stderr);
    // fprintf(stderr, "node -> checksum = %d\n", node->checksum);
    // fflush(stderr);
    // fprintf(stderr, "Purpose -> pointer : %p", Purpose + Index );
    // fflush(stderr);
    (Purpose + Index)->hash = node->checksum;
    (Purpose + Index)->string = node->token;
    Index++;
    // fprintf(stderr, "Purpose = %d\n", (Purpose + Index)->hash);
    // fflush(stderr);
    if (node->left != NULL) {
        GiveChecksum(Purpose, node->left, 1);
    }
    if (node->right != NULL) {
        GiveChecksum(Purpose, node->right, 1);
    }
}

bool isToken(char* token, char* string) {
    // fprintf(stderr, "string = %s\n",string);
    // fflush(stderr);
    int k = strlen(token);
    int j = strlen(string);
    if (k != j)
        return false;
    else {
        return caseInsensitiveCmp(token, string, k) ;
    }
}

bool isHash(Unsigned* Purpose, Node* mail, int position) {
    // fprintf(stderr, "checksum = %d\n", mail -> checksum);
    // fprintf(stderr, "Hash = %d\n", Purpose->hash);

    // fflush(stderr);
    if (Purpose->hash == mail->checksum){
        // fprintf(stderr, "checksum = %d\n", mail -> checksum);
        // fprintf(stderr, "Hash = %d\n", Purpose->hash);
        return isToken(mail->token, Purpose->string);
    }
    else if (mail->left != NULL || mail->right != NULL) {
        if (mail->left == NULL)
            return isHash(Purpose, mail->right, position);
        else if (mail->right == NULL)
            return isHash(Purpose, mail->left, position);
        else
            return (isHash(Purpose, mail->left, position) || isHash(Purpose, mail->right, position));
    } else
        return false;
}

double CompareChecksum(Unsigned* Purpose, Node* mail, int Length) {
    // Purpose is Token's hashs, mail is the mail[i], Length is the size of Purpose
    bool K;
    int AND = 0;
    int mail_size = 0;
    double Similarity = 0;
    for (int i = 0; i < Length; i++) {
        K = isHash(Purpose + i, mail->right, i);
        // fprintf(stderr, "isHash = %d\n",K);
        // fflush(stderr);
        if (K != false) AND++;
        else {
            // fprintf(stderr, "i = %d\n", i);
            // fprintf(stderr, "string = %s\n", (Purpose+i)->string);
            // fprintf(stderr, "token = %s\n", (mail->right)->token);
        }
        // fprintf(stderr, "AND = %f \n", AND);
        // fflush(stderr);
    }
    mail_size = Size(mail->right);
    if ( AND != 0 ){
        fprintf(stderr, "AND = %d\n", AND);
    }
    fprintf(stderr, "mail_size = %d\n", mail_size);
    fprintf(stderr, "Length = %d\n", Length);
    Similarity =(double) AND / (Length + mail_size - AND);
    fprintf(stderr, "Similarity = %f\n", Similarity);
    return Similarity;
};

void findSimilar(int mid, double threshold, int* answers, int* answerLength,
                 Node** mailTokenBSTs) {
    // similar = (A ∩ B)/(A U B)
    // mailTokenBSTs = forest
    Node* PurposeMail = mailTokenBSTs[mid];
    // printBST(PurposeMail);
    double PurposeSimilar = 0;
    int TokenSize = Size(mailTokenBSTs[mid]->right);  // find purpose mail's Token
    int Length = 0;                               // return mail amount
    Unsigned Purpose[TokenSize];                  // store purpose mail's every Token
    GiveChecksum(Purpose, PurposeMail->right,0);          // get checksum from purpose mail
    // for (int i = 0; i < TokenSize; i++) {
    //     fprintf(stderr, "Purpose[%d] = %d\n", i, (Purpose + i)->hash);
    // }
    // fprintf(stderr, "TokenSize = %d \n", TokenSize);
    // fflush(stderr);
    // fprintf(stderr,"Mid = %d \n",mid );
    for (int i = 5209; i < 5210; i++) {  // mails[i]
        if (i == mid) continue;
        // fprintf(stderr, "token = %s\n", mailTokenBSTs[1]->right->token);
        // fprintf(stderr, "string = %s\n", (Purpose + 0)->string);
        fprintf(stderr, "compare to: %d\n", i);
        PurposeSimilar = CompareChecksum(Purpose, mailTokenBSTs[i], TokenSize);
        // fprintf(stderr, "TRYTRYTRY\n");
        // fflush(stderr);
        
        if (PurposeSimilar > threshold) {
            // fprintf(stderr,"K_Thresold = %f \n",PurposeSimilar );
            // fflush(stderr);
            // fprintf(stderr, "PurposeSimilar[%d] = %f \n", i, PurposeSimilar);
            // fflush(stderr);
            answers[Length] = i;
            Length++;
        }
    }
    *answerLength = Length;
    // fprintf(stderr, "here\n");
    // fflush(stderr);
}