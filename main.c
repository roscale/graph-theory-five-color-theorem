#include <stdlib.h>
#include <stdio.h>
#include "Stack.h"
#include "graph.h"

int main() {
	Stack *s4 = createStack(100);
	Stack *s5 = createStack(100);
	Stack *sd = createStack(100);

	GRAPHE *graph = malloc(sizeof(GRAPHE));
	initialiserGraphe(graph);

	int a = ajouterSommet(graph, 1);
	int b = ajouterSommet(graph, 2);
	int c = ajouterSommet(graph, 3);

	// Returns 0 on success
	addEdge(graph, a, b, 0);
	addEdge(graph, a, c, 0);
	addEdge(graph, b, c, 0);

	printf("Initial graph:\n");
	afficherGraphe(graph);

	// TODO remove loops
	printf("Vertex list:\n");
	for (SOMMET *v = graph->premierSommet; v != NULL; v = v->suivant) {
		// Degree at most 4
		if (v->degree <= 4) {
			pushStack(s4, v);
		}
			// Degree five
		else if (v->degree == 5) {
			// and at least one adjacent vertex with degree at most six
			for (ELTADJ *adj = v->adj; adj != NULL; adj = adj->suivant) {
				printf("Neighbour of %i: %i\n", v->label, adj->dest);
			}
		}
	}

	return 0;
}