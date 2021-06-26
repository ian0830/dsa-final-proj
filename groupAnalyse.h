#include <stdio.h>

#include "api.h"
#include "nameBst.h"

int n_mails, n_queries;
mail *mails;
query *queries;

typedef struct graphNode {
    char *name;
    struct graphNode *next;
    // For DFS use
    int color;  // color: 0:White; 1:Gray; 2:Black
    int discover;
    int finish;
    struct graphNode *parentNode;

} GraphNode;

typedef struct graph {
    int numVertices;
    GraphNode **graphNodeList;
} Graph;

Graph *graph_init(int vertices) {
    // The size of graph->nodeList is 2 * vertices
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->numVertices = vertices;
    graph->graphNodeList =
        (GraphNode *)malloc(sizeof(GraphNode) * vertices * 2);

    for (int i = 0; i < vertices; i++) {
        graph->graphNodeList[i] = NULL;
    }
    return graph;
}

GraphNode *graphNode_init(char *name) {
    GraphNode *node = (GraphNode *)malloc(sizeof(GraphNode));
    node->name = name;
    node->next = NULL;
    node->color = NULL;
    node->discover = NULL;
    node->finish = NULL;
    node->parentNode = NULL;
    return node;
}

void addEdge(Graph *graph, char *from, char *to, Node *nameBST) {
    // Add edge from "from" to "to"
    GraphNode *graphNode = graphNode_init(to);
    int fromIndex = searchBST(nameBST, from);
    graphNode->next = graph->graphNodeList[fromIndex];
    graph->graphNodeList[fromIndex] = graphNode;
    // Add edge from "to" to "from"
    graphNode = graphNode_init(from);
    int toIndex = searchBST(nameBST, to);
    graphNode->next = graph->graphNodeList[toIndex];
    graph->graphNodeList[toIndex] = graphNode;
}

void DFSVisit(GraphNode *graphNode, int time, int index, char **IndexToName, Node *BST) {
    time = time + 1;
    graphNode->discover = time;
    graphNode->color = 1;
    for (graphNode = graphNode->next; graphNode != NULL;
         graphNode = graphNode->next) {
        if (graphNode->color == 0) {
            graphNode->parentNode = searchBST(BST, IndexToName[index]);
            DFSVisit(graphNode, time, index, IndexToName, BST);
        }
    }
    graphNode->color = 2;
    time = time + 1;
    graphNode->finish = time;
}

int DFS(Graph *graph, int len, Node *BST, char **IndexToName, int *answers) {
    len = 2 * len;
    for (int i = 0; i < len; i++) {
        if (graph->graphNodeList[i] != NULL) {
            graph->graphNodeList[i]->color = 0;
            graph->graphNodeList[i]->parentNode = 0;  // NIL
        }
    }
    int time = 0;
    int numOfGroups = 0;
    int max = 0;
    for (int index = 0; index < len; index++) {
        if (graph->graphNodeList[index] != NULL) {
            if (graph->graphNodeList[index]->color == 0) {
                numOfGroups = numOfGroups + 1;
                int timestart = time;
                DFSVisit(graph->graphNodeList[index], time, index, IndexToName, BST);
                int groupSize = (time - timestart) / 2;
                if (groupSize > max) {
                    max = groupSize;
                }
            }
        }
    }
    answers[0] = numOfGroups;
    answers[1] = max;
    return answers;
}

void groupAnalyse(int len, int *mids, int **answers, int *answerLength) {
    Graph *graph = graph_init(len);
    Node *nameBST = generateNameBST(mails, len, mids);
    char IndexToName[2 * len];
    inorderBST(nameBST, IndexToName);

    for (int i = 0; i < len; i++) {
        addEdge(graph, mails->from, mails->to, nameBST);
    }

    DFS(graph, len, nameBST, IndexToName, answers);
    //answerLenghth is 2 (default and fixed)
    return answers;
}