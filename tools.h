#ifndef TOOLS_H
#define TOOLS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "Stack.h"
#include "graph.h"

/*
 * Checks whether a vertex matches the conditions to be in the s4 stack
 */
bool vertexMatchesConditionsForS4(SOMMET *vertex);

/*
 * Checks whether a vertex matches the conditions to be in the s5 stack
 */
bool vertexMatchesConditionsForS5(SOMMET *vertex);

/*
 * Iterate the initial graph and populate the s4 and s5 stacks
 */
void populateStacks(GRAPHE *graph, Stack *s4, Stack *s5);

/*
 * Pops a vertex from s5 (v) and merges 2 of its neighbours (v1 and v2) into one single vertex.
 * The merge consists of:
 * 1) Deleting v from the graph
 * 2) Transferring all the neighbours of v2 to v1
 * 3) Deleting v2 from the graph
 * 4) Checking and transferring all the affected vertices in their corresponding stacks (s4 or s5).
 */
void mergeVertices(GRAPHE *graph, Stack *s4, Stack *s5, Stack *sd);

/*
 * Pops every vertex from s4, deletes them from the graph, pushes them to sd.
 * Checks and transfers all the affected neighbours in their corresponding stacks (s4 or s5).
 */
void stack4ToStackD(GRAPHE *graph, Stack *s4, Stack *sd, Stack *s5);

/*
 * Gets the first color available from an array of used colors.
 */
Color getColorFromArray(Color *colors);

#endif //TOOLS_H
