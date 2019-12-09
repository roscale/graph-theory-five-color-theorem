#include <stdlib.h>
#include <stdio.h>
#include "Stack.h"
#include "graph.h"

//https://en.wikipedia.org/wiki/Five_color_theorem?fbclid=IwAR2gWXQwMi2bhCh9HIW6gnY_vz_qEf5Cwk5j-c-YYZk23LTf-KPZSMsBNJY
int main() {
	Stack *s4 = createStack(100);
	Stack *s5 = createStack(100);
	Stack *sd = createStack(100);

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

	// TODO remove loops
	printf("Vertex list:\n");
	for (SOMMET *v = graph->premierSommet; v != NULL; v = v->suivant) {
		// Degree at most 4
		if (v->degree <= 4) {
			pushStack(s4, v);
			printf("We push. Degree : %d \n", v->degree);
		}
			// Degree five
		else if (v->degree == 5) {
			// and at least one adjacent vertex with degree at most six
			for (ELTADJ *adj = v->adj; adj != NULL; adj = adj->suivant) {
				printf("Neighbour of %i: %i\n", v->label, adj->dest);
			}
		}
	}
	// Test push raul

	return 0;
}