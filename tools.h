#ifndef TOOLS_H
#define TOOLS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "Stack.h"
#include "graph.h"

SOMMET *findVertexWithLabel(GRAPHE *graph, int label);

bool vertexMatchesConditionsForS4(SOMMET *vertex);

bool vertexMatchesConditionsForS5(GRAPHE *graph, SOMMET *vertex);

void populateStacks(GRAPHE *graph, Stack *s4, Stack *s5);

void mergeVertices(GRAPHE *graph, Stack *s5, Stack *sd);

void stack4ToStackD(GRAPHE *graph, Stack *s4, Stack *sd, Stack *s5);

#endif //TOOLS_H
