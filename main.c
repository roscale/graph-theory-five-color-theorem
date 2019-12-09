#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "Stack.h"
#include "graph.h"
#include "tools.h"

// https://en.wikipedia.org/wiki/Five_color_theorem?fbclid=IwAR2gWXQwMi2bhCh9HIW6gnY_vz_qEf5Cwk5j-c-YYZk23LTf-KPZSMsBNJY


int main() {
	Stack *s4 = createStack(100);
	Stack *s5 = createStack(100);
	Stack *sd = createStack(100);

	// Graph example
	GRAPHE *graph = malloc(sizeof(GRAPHE));
	initialiserGraphe(graph);

	// We should try this example in the end
	// https://i.stack.imgur.com/rO3SR.png
	// It is a planar graph with a minimum degree of 5

	Position pa = {0, 0};
	Position pb = {-1, -1};
	Position pc = {1, -1};

	SOMMET *a = ajouterSommet(graph, pa);
	SOMMET *b = ajouterSommet(graph, pb);
	SOMMET *c = ajouterSommet(graph, pc);

	// Returns 0 on success
	addEdge(graph, a, b);
	addEdge(graph, a, c);
	addEdge(graph, b, c);

	printf("Initial graph:\n");
	afficherGraphe(graph);

	// STEP 1
	populateStacks(graph, s4, s5);

	// STEP 2
	printf("----------In step2\n");
//	stack4ToStackD(graph, s4, sd, s5);
	printf("Out step2\n");

	// STEP 3
//	assert(isStackEmpty(s4)); // deg(graph) >= 5

	// The graph is empty
	if (graph->nbS == 0) {
		// STEP 5
		// TODO
	} else {
//		mergeVertices(graph, s5, sd);
	}

	return 0;
}