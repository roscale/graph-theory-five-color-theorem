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

	int a = ajouterSommet(graph, 1);
	int b = ajouterSommet(graph, 2);
	int c = ajouterSommet(graph, 3);
	int d = ajouterSommet(graph, 4);
	int e = ajouterSommet(graph, 5);
	int f = ajouterSommet(graph, 6);

	// Returns 0 on success
	addEdge(graph, a, b, 0);
	addEdge(graph, a, c, 0);
	addEdge(graph, b, c, 0);

	addEdge(graph, a, f, 0);
	addEdge(graph, a, e, 0);
	addEdge(graph, a, d, 0);
	addEdge(graph, c, d, 0);
	addEdge(graph, b, d, 0);
	addEdge(graph, d, f, 0);
	addEdge(graph, e, f, 0);

	printf("Initial graph:\n");
	afficherGraphe(graph);

	// STEP 1
	populateStacks(graph, s4, s5);

	// STEP 2
	// TODO

	// STEP 3
	assert(isStackEmpty(s4)); // deg(graph) >= 5

	// The graph is empty
	if (graph->nbS == 0) {
		// STEP 5
		// TODO
	} else {
		mergeVertices(graph, s5, sd);
	}

	return 0;
}