#include <stdio.h>

#include "api.h"
#include "expressionMatch.h"
#include "findSimilar.h"
#include "groupAnalyse.h"
#include "tokenBst.h"

// The testdata only contains the first 100 mails (mail1 ~ mail100)
// and 2000 queries for you to debug.

int n_mails, n_queries;
mail *mails;
query *queries;

Node **mailTokenBSTs;

int main(void) {
    api.init(&n_mails, &n_queries, &mails, &queries);

    mailTokenBSTs = generateTokenBSTs(mails, n_mails);

    for (int i = 0; i < n_queries; i++) {
        int answerLength;
        int *answers;
        if (queries[i].type == expression_match) {
            expressionMatch(queries[i].data.expression_match_data.expression,
                            &answers, &answerLength, mailTokenBSTs);
            api.answer(queries[i].id, answers, answerLength);
        }
        if (queries[i].type == find_similar) {
            findSimilar(queries[i].data.find_similar_data.mid,
                        queries[i].data.find_similar_data.threshold, &answers,
                        &answerLength, mailTokenBSTs);
            api.answer(queries[i].id, answers, answerLength);
        }
        if (queries[i].type == group_analyse) {
            groupAnalyse(queries[i].data.group_analyse_data.len,
                         queries[i].data.group_analyse_data.mids, &answers,
                         &answerLength);
            api.answer(queries[i].id, answers, answerLength);
        }
    }

    return 0;
}
