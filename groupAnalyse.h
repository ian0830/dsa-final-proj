#include <stdio.h>

#include "api.h"
#include "nameBst.h"

int n_mails, n_queries;
mail *mails;
query *queries;

typedef struct graphNode {
    char *name;
    struct graphNode *next;
} GraphNode;

typedef struct dfsProperties {
    int index;
    int color;  //0: White; 1:Gray; 2:Black
    int discover;
    int finish;
    GraphNode *parentNode;
} DFSProperties;

typedef struct graph {
    int numVertices;
    GraphNode **graphNodeList;
    DFSProperties **DFSstate;
} Graph;

Graph *graph_init(int vertices) {
    // The size of graph->nodeList is 2 * vertices
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->numVertices = vertices;
    int temp = (vertices + 1) * 2;
    graph->graphNodeList = (GraphNode *)malloc(sizeof(GraphNode) * temp);

    for (int i = 0; i < temp; i++) {
        graph->graphNodeList[i] = NULL;
    }

    graph->DFSstate = (DFSProperties *)malloc(sizeof(DFSProperties) * temp);
    for (int j = 0; j < temp; j++) {
        DFSProperties *dfsProperties = (DFSProperties *)malloc(sizeof(DFSProperties));
        dfsProperties->index = j;
        dfsProperties->color = NULL;
        dfsProperties->discover = NULL;
        dfsProperties->finish = NULL;
        dfsProperties->parentNode = NULL;
        graph->DFSstate[j] = dfsProperties;
    }
    return graph;
}

GraphNode *graphNode_init(char *name) {
    GraphNode *node = (GraphNode *)malloc(sizeof(GraphNode));
    node->name = name;
    node->next = NULL;
    return node;
}

void IndexToName_init(char **array, int len) {
    int temp = (len + 1) * 2;
    for (int i = 0; i <= temp; i++) {
        array[i] = NULL;
    }
}

void addEdge(Graph *graph, char *from, char *to, NameNode *nameBST) {
    // Add edge from "from" to "to"
    GraphNode *graphNode = graphNode_init(to);
    int fromIndex = searchBST(nameBST, from)->index;
    if (graph->graphNodeList[fromIndex] == NULL) {
        graphNode->next = graph->graphNodeList[fromIndex];
        graph->graphNodeList[fromIndex] = graphNode;
    } else {
        GraphNode *graphNodeTracker = graph->graphNodeList[fromIndex];
        while (graphNodeTracker->next != NULL) {
            graphNodeTracker = graphNodeTracker->next;  //To find the node before NULL
        }
        graphNode->next = graphNodeTracker->next;
        graphNodeTracker->next = graphNode;
    }

    // Add edge from "to" to "from"
    graphNode = graphNode_init(from);
    int toIndex = searchBST(nameBST, to)->index;
    if (graph->graphNodeList[toIndex] == NULL) {
        graphNode->next = graph->graphNodeList[toIndex];
        graph->graphNodeList[toIndex] = graphNode;
    } else {
        GraphNode *graphNodeTracker = graph->graphNodeList[toIndex];
        while (graphNodeTracker->next != NULL) {
            graphNodeTracker = graphNodeTracker->next;
        }
        graphNode->next = graphNodeTracker->next;
        graphNodeTracker->next = graphNode;
    }
}

// void DFSVisit(GraphNode *graphNode, int time, int index, char **IndexToName, NameNode *BST) {
//     fprintf(stderr, "DFSVisit\n");
//     time = time + 1;
//     graphNode->discover = time;
//     graphNode->color = 1;
//     for (graphNode = graphNode->next; graphNode != NULL;
//          graphNode = graphNode->next) {
//         if (graphNode->color == 0) {
//             graphNode->parentNode = searchBST(BST, IndexToName[index]);
//             DFSVisit(graphNode, time, index, IndexToName, BST);
//         }
//     }
//     graphNode->color = 2;
//     time = time + 1;
//     graphNode->finish = time;
// }

//Problem in the DFS now
int recorder = 0;

void newDFSVisit(Graph *graph, int index, NameNode *nameBST) {
    recorder = recorder + 1;
    graph->DFSstate[index]->discover = recorder;
    graph->DFSstate[index]->color = 1;
    if (graph->graphNodeList[index] != NULL) {
        GraphNode *graphNodeTracker = graph->graphNodeList[index];
        while (graphNodeTracker != NULL) {
            int vIndex = searchBST(nameBST, graphNodeTracker->name)->index;
            if (graph->DFSstate[vIndex]->color == 0) {
                graph->DFSstate[vIndex]->parentNode = graph->graphNodeList[index];
                //fprintf(stderr, "here %s %d\n", graph->graphNodeList[index]->name, recorder);
                newDFSVisit(graph, vIndex, nameBST);
            }
            graphNodeTracker = graphNodeTracker->next;
        }
    }
    graph->DFSstate[index]->color = 2;
    recorder = recorder + 1;
    graph->DFSstate[index]->finish = recorder;
}

int DFS(Graph *graph, int len, NameNode *nameBST, char **IndexToName, int *answers) {
    int temp = (len + 1) * 2;
    for (int i = 0; i < temp; i++) {
        if (graph->graphNodeList[i] != NULL) {
            graph->DFSstate[i]->color = 0;
            graph->DFSstate[i]->parentNode = NULL;  //NIL
        }
    }

    // int time = 0;
    int numOfGroups = 0;
    int max = 1;
    for (int index = 0; index < temp; index++) {
        if (graph->graphNodeList[index] != NULL) {
            if (graph->DFSstate[index]->color == 0) {  //color = whilte
                numOfGroups = numOfGroups + 1;
                int timeStart = recorder;
                //fprintf(stderr, "======VisitNewGroup======\n");
                newDFSVisit(graph, index, nameBST);
                int groupSize = (recorder + 1 - timeStart) / 2;
                //fprintf(stderr, "groupSize: %d %d %d\n", groupSize, recorder, timeStart);
                if (groupSize > max) {
                    max = groupSize;
                }
            }
        }
    }

    //answerLenghth is 2 (default and fixed)
    answers[0] = numOfGroups;
    answers[1] = max;
    // fprintf(stderr, "number of Groups: %d\n", answers[0]);
    // fprintf(stderr, "number of max group: %d\n", answers[1]);
}

void graphPrint(Graph *graph, int len, char **array) {
    fprintf(stderr, "====start graph print======\n");
    int temp = (len + 1) * 2;
    for (int i = 0; i < temp; i++) {
        fprintf(stderr, "graphNodeList[%d]: %s\n", i, array[i]);
        //use pointer to point to the graph.graphNodeList[i]
        GraphNode *graphNodeTracker = graph->graphNodeList[i];
        fprintf(stderr, "Adjacent nodes: ");
        //fprintf(stderr,"graphNodeTracker: %s %s\n", graphNodeTracker->name, graphNodeTracker->next->name);
        while (graphNodeTracker != NULL) {
            fprintf(stderr, "%s ", graphNodeTracker->name);
            graphNodeTracker = graphNodeTracker->next;
        }
        fprintf(stderr, "\n");
    }
}

void printInorderBSTResult(char **array, int len) {
    int temp = (len + 1) * 2;
    for (int i = 0; i < temp; i++) {
        if (array[i] != NULL)
            fprintf(stderr, "array[%d]: %s\n", i, array[i]);
    }
}

void groupAnalyse(int len, int *mids, int *answers, int *answerLength) {
    
    Graph *graph = graph_init(len);
    NameNode *nameBST = generateNameBST(mails, len, mids);

    char *IndexToName[2 * len];
    IndexToName_init(IndexToName, len);
    inorderBST(nameBST, IndexToName);
    // printInorderBSTResult(IndexToName, len);

    for (int id = 0; id < len; id++) {
        mail *m = mails + mids[id];
        addEdge(graph, m->from, m->to, nameBST);
    }
    // graphPrint(graph, len, IndexToName);
    DFS(graph, len, nameBST, IndexToName, answers);
    // fprintf(stderr, "=================DSA FUCKUP================\n");
    // fprintf(stderr, "number of Groups: %d\n", answers[0]);
    // fprintf(stderr, "number of max group: %d\n", answers[1]);
    *answerLength = 2;
}