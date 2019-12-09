#include "tools.h"

const char *colorToString(Color color) {
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

void stack4ToStackD(GRAPHE *graph, Stack *s4, Stack *sd, Stack *s5) {

	for (SOMMET *v = graph->premierSommet; v != NULL; v = v->suivant) {
		for (ELTADJ *padj = v->adj; padj != NULL; padj = padj->suivant) {
			printf("Dest : %d\n",padj->dest );
			SOMMET *neighbour = findVertexWithLabel(graph, padj->dest);
			if (vertexMatchesConditionsForS4(neighbour)) {
			pushStack(s4, neighbour);
			}
			else if (vertexMatchesConditionsForS5(graph, neighbour)) {
			pushStack(s5, neighbour);
			}
		}
		pushStack(sd, v); 
		graph->premierSommet = v->suivant;
		
	}
}

void mergeVertices(GRAPHE *graph, Stack *s5, Stack *sd) {
	assert(!isStackEmpty(s5));
	SOMMET *v = popStack(s5);
}