#include "api.h"
#include "tokenBst.h"
#include "expressionMatch.h"
#include "findSimilar.h"
#include "groupAnalyse.h"

// The testdata only contains the first 100 mails (mail1 ~ mail100)
// and 2000 queries for you to debug.

int n_mails, n_queries;
mail *mails;
query *queries;

Node **mailTokenBSTs;

// tested on 06/26 00:30
// void printBST(Node *node) {
//     if (node == NULL) {
//         return;
//     }
//     printBST(node->left);
//     fprintf(stderr, "%s(%d) ", node->token, node->checksum);
//     printBST(node->right);
// }

int main(void) {
    api.init(&n_mails, &n_queries, &mails, &queries);

    mailTokenBSTs = generateTokenBSTs(mails, n_mails);
    // printBST(mailTokenBSTs[0]);

    for (int i = 0; i < n_queries; i++) {
        int answerLength;
        int answers[n_mails];
        if (queries[i].type == expression_match) {
            expressionMatch(queries[i].data.expression_match_data.expression, n_mails,
                            answers, &answerLength, mailTokenBSTs);
            
            fprintf(stderr, "answerLength: %d\n", answerLength);
           
            api.answer(queries[i].id, answers, answerLength);
        }
        // if (queries[i].type == find_similar) {
        //     findSimilar(queries[i].data.find_similar_data.mid,
        //                 queries[i].data.find_similar_data.threshold, &answers,
        //                 &answerLength, mailTokenBSTs);
        //     api.answer(queries[i].id, answers, answerLength);
        // }
        // if (queries[i].type == group_analyse) {
        //     groupAnalyse(queries[i].data.group_analyse_data.len,
        //                  queries[i].data.group_analyse_data.mids, &answers,
        //                  &answerLength);
        //     api.answer(queries[i].id, answers, answerLength);
        // }
    }

    return 0;
}
