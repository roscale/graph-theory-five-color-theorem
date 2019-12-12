#include "tools.h"

bool vertexMatchesConditionsForS4(SOMMET *vertex) {
	// Degree at most 4
	return vertex->degree <= 4;
}

bool vertexMatchesConditionsForS5(GRAPHE *graph, SOMMET *vertex) {
	// Degree five
	if (vertex->degree == 5) {
		// and at least one adjacent vertex with degree at most six
		ELTADJ *adj = vertex->adj;
		for (size_t i = 0; adj != NULL && i < vertex->adjSize; i++, adj = adj->suivant) {
			printf("Neighbour of %i: %i\n", vertex->label, adj->vertex->label);

			if (adj->vertex->degree <= 6) {
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

		ELTADJ *padj = v->adj;
		for (size_t i = 0; padj != NULL && i < v->adjSize; i++, padj = padj->suivant) {
			// printf("Dest : %d, info : %d\n",padj->dest, padj->info);
			SOMMET *neighbour = padj->vertex;
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
		supprimerSommet(graph, v);
		pushStack(sd, v);
	}
}

// Inefficient but it works and I don't care.
static bool areNeighbours(SOMMET *v1, SOMMET *v2) {
	ELTADJ *adj = v1->adj;
	for (size_t i = 0; i < v1->adjSize; i++) {
		if (adj->vertex == v2) {
			return true;
		}
		adj = adj->suivant;
	}
	return false;
}

void mergeVertices(GRAPHE *graph, Stack *s5, Stack *sd) {
	assert(!isStackEmpty(s5));
	if (graph->nbS == 0) {
		// The graph is empty, go to step 5
		return;
	}

	SOMMET *v = popStack(s5);

	// TODO We shouldn't delete it here, the splicing will always fail
	supprimerSommet(graph, v);
	pushStack(sd, v);

	////////
	ELTADJ *adj = v->adj;
	for (size_t i = 0; i < v->adjSize; i++) {
		printf("Label: %d, degree: %d\n",
		       adj->vertex->label, adj->vertex->degree);

		adj = adj->suivant;
	}
	////////

	SOMMET *v1 = v->adj->vertex;
	SOMMET *v3 = v->adj->suivant->suivant->vertex;

	// if v1 is NOT adjacent to v3
	if (!areNeighbours(v1, v3)) {
		spliceLists(v1, v3, v);
		v1->mergedWith = v3;
		pushStack(sd, v3);
		supprimerSommet(graph, v1);

		// TODO?
		// It's possible that this might create faces bounded by two edges at the two points where the lists are spliced together; we delete one edge from any such faces.


	} else {
		// v2 and v4 are not adjacent
		SOMMET *v2 = v->adj->suivant->vertex;
		SOMMET *v4 = v->adj->suivant->suivant->suivant->vertex;

		spliceLists(v4, v2, v);
		v4->mergedWith = v2;
		pushStack(sd, v4);
	}
}


Color getColorFromArray(Color *colors){
	assert(colors != NULL);
	for (int i = 0; i < 5; ++i)
	{
			// printf("colors[%d] : %d ; colors[i] >= 0 : %d; %d\n",i, colors[i], ((int)colors[i]) >= 0, -1 > 1);
		if ((int)colors[i] >= 0)
		{
			 return colors[i];
		}
	}

	return colors[0];
}