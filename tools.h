#ifndef TOOLS_H
#define TOOLS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "Stack.h"
#include "graph.h"

typedef enum Color_t {
   RED,
   GREEN,
   BLUE,
   YELLOW,
   PURPLE
}Color;

const char *colorToString(Color color);

SOMMET *findVertexWithLabel(GRAPHE *graph, int label);

bool vertexMatchesConditionsForS4(SOMMET *vertex);

bool vertexMatchesConditionsForS5(GRAPHE *graph, SOMMET *vertex);

void populateStacks(GRAPHE *graph, Stack *s4, Stack *s5);

void mergeVertices(GRAPHE *graph, Stack *s5, Stack *sd);

#endif //TOOLS_H
