#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "Stack.h"
#include "graph.h"

// https://en.wikipedia.org/wiki/Five_color_theorem?fbclid=IwAR2gWXQwMi2bhCh9HIW6gnY_vz_qEf5Cwk5j-c-YYZk23LTf-KPZSMsBNJY

enum Color {
   RED,
   GREEN,
   BLUE,
   YELLOW,
   PURPLE
};

const char *colorToString(int color) {
	switch (color) {
		case RED: return "RED";
		case GREEN: return "GREEN";
		case BLUE: return "BLUE";
		case YELLOW: return "YELLOW";
		case PURPLE: return "PURPLE";
		default: assert(false && "Invalid color");
	}
}

// TODO Make the adjacency list have pointers to vertices, not labels.
// Otherwise, we have a O(n^2) complexity
SOMMET *findVertexWithLabel(GRAPHE *graph, int label) {
	SOMMET *foundVertex = NULL;
	for (SOMMET *v = graph->premierSommet;
	     v != NULL; v = v->suivant) {
		// Search the vertex with the same label
		// Each vertex has a unique label
		if (v->label == label) {
			foundVertex = v;
			break;
		}
	}
	assert(foundVertex != NULL);
	return foundVertex;
}

bool vertexMatchesConditionsForS4(SOMMET *vertex) {
	// Degree at most 4
	return vertex->degree <= 4;
}

bool vertexMatchesConditionsForS5(GRAPHE *graph, SOMMET *vertex) {
	// Degree five
	if (vertex->degree == 5) {
		// and at least one adjacent vertex with degree at most six
		for (ELTADJ *adj = vertex->adj; adj != NULL; adj = adj->suivant) {
			SOMMET *neighbour = findVertexWithLabel(graph, adj->dest);
			printf("Neighbour of %i: %i\n", vertex->label, neighbour->label);

			if (neighbour->degree <= 6) {
				return true;
			}
		}
	}
	return false;
}

void populateStacks(GRAPHE *graph, Stack *s4, Stack *s5) {
	for (SOMMET *v = graph->premierSommet; v != NULL; v = v->suivant) {
		if (vertexMatchesConditionsForS4(v)) {
			pushStack(s4, v);
		}
		else if (vertexMatchesConditionsForS5(graph, v)) {
			pushStack(s5, v);
		}
	}
}

void mergeVertices(GRAPHE *graph, Stack *s5, Stack *sd) {
	assert(!isStackEmpty(s5));
	SOMMET *v = popStack(s5);
}

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