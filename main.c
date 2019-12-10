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

	// Graph example 1
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

	// Graph exemple 2 ( 5-complet )
	GRAPHE *graph2 = malloc(sizeof(GRAPHE));
	initialiserGraphe(graph2);

	int a2 = ajouterSommet(graph2, 1);
	int b2 = ajouterSommet(graph2, 2);
	int c2 = ajouterSommet(graph2, 3);
	int d2 = ajouterSommet(graph2, 4);
	int e2 = ajouterSommet(graph2, 5);
	int f2 = ajouterSommet(graph2, 6);
	int g2 = ajouterSommet(graph2, 7);
	int h2 = ajouterSommet(graph2, 8);
	int i2 = ajouterSommet(graph2, 9);
	int j2 = ajouterSommet(graph2, 10);
	int k2 = ajouterSommet(graph2, 11);
	int l2 = ajouterSommet(graph2, 12);

	// Returns 0 on success
	addEdge(graph2, a2, b2, 0);
	addEdge(graph2, a2, c2, 0);
	addEdge(graph2, a2, d2, 0);
	addEdge(graph2, a2, i2, 0);
	addEdge(graph2, a2, h2, 0);

	addEdge(graph2, b2, d2, 0);
	addEdge(graph2, b2, e2, 0);
	addEdge(graph2, b2, f2, 0);
	addEdge(graph2, b2, c2, 0);

	addEdge(graph2, c2, f2, 0);
	addEdge(graph2, c2, g2, 0);
	addEdge(graph2, c2, h2, 0);

	addEdge(graph2, i2, j2, 0);
	addEdge(graph2, i2, l2, 0);
	addEdge(graph2, i2, d2, 0);
	addEdge(graph2, i2, h2, 0);

	addEdge(graph2, d2, j2, 0);
	addEdge(graph2, d2, e2, 0);

	addEdge(graph2, e2, j2, 0);
	addEdge(graph2, e2, k2, 0);
	addEdge(graph2, e2, f2, 0);

	addEdge(graph2, f2, k2, 0);
	addEdge(graph2, f2, g2, 0);

	addEdge(graph2, g2, k2, 0);
	addEdge(graph2, g2, l2, 0);
	addEdge(graph2, g2, h2, 0);

	addEdge(graph2, h2, l2, 0);

	addEdge(graph2, l2, j2, 0);
	addEdge(graph2, l2, k2, 0);

	addEdge(graph2, k2, j2, 0);

	printf("Initial graph:\n");
	// afficherGraphe(graph);
	afficherGraphe(graph2);

	// STEP 1
	// populateStacks(graph, s4, s5);
	populateStacks(graph2, s4, s5);

	// STEP 2
	printf("----------In step2\n");
	printf("Inside s4 : ");
	for (int i = 0; i < s4->size; ++i)
	{
		printf("%d ",s4->vertices[i]->label );
	}
	printf("\n");

	// stack4ToStackD(graph, s4, sd, s5);
	stack4ToStackD(graph2, s4, sd, s5);
	printf("----------Out step2\n");

	printf("Graph after step2:\n");
	// afficherGraphe(graph);
	afficherGraphe(graph2);

	// STEP 3
	assert(isStackEmpty(s4)); // deg(graph) >= 5

	// The graph is empty
	if (graph->nbS == 0) {
		// STEP 5
		// TODO
	} else {
		// mergeVertices(graph, s5, sd);
		mergeVertices(graph, s5, sd);
	}

	return 0;
}