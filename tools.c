#include "tools.h"

// TODO Make the adjacency list have pointers to vertices, not labels.
// Otherwise, we have a O(n^2) complexity
SOMMET *findVertexWithLabel(GRAPHE *graph, int label) {
	SOMMET *foundVertex = NULL;
	for (SOMMET *v = graph->premierSommet;
	     v != NULL; v = v->suivant) {
		// Search the vertex with the same label
		// Each vertex has a unique label
		// printf("Actual label : %d\n",v->label );
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

	SOMMET *v;
	while (!isStackEmpty(s4)) {
		// printf("Inside s4 : ");
		// for (int i = 0; i < s4->size; ++i)
		// {
		// 	printf("%d ",s4->vertices[i]->label );
		// }
		// printf("\n");
		v = popStack(s4);
		for (ELTADJ *padj = v->adj; padj != NULL; padj = padj->suivant) {
			// printf("Dest : %d, info : %d\n",padj->dest, padj->info);
			SOMMET *neighbour = findVertexWithLabel(graph, padj->dest);
			if (vertexMatchesConditionsForS4(neighbour) && !isVertexInStack(s4,neighbour)) {
			pushStack(s4, neighbour);
			}
			else if (vertexMatchesConditionsForS5(graph, neighbour)&& !isVertexInStack(s5,neighbour)) {
			pushStack(s5, neighbour);
			}
		}
		// printf("Out\n");
		// printf("Inside s4 after : ");
		// for (int i = 0; i < s4->size; ++i)
		// {
		// 	printf("%d ",s4->vertices[i]->label );
		// }
		// printf("\n");
		supprimerSommet(graph, v->label);
		pushStack(sd, v);
		
	}
}

void mergeVertices(GRAPHE *graph, Stack *s5, Stack *sd) {
	assert(!isStackEmpty(s5));
	SOMMET *v = popStack(s5);
}