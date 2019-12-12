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
		supprimerSommet(graph, v);
		pushStack(sd, v);

		ELTADJ *padj = v->adj;
		for (size_t i = 0; padj != NULL && i < v->adjSize; i++, padj = padj->suivant) {
			// printf("Dest : %d, info : %d\n",padj->dest, padj->info);

			SOMMET *neighbour = padj->vertex;
			if (vertexMatchesConditionsForS4(neighbour)) {
				removeElementFromStack(s5, neighbour);
				if (!isVertexInStack(s4,neighbour)) {
					pushStack(s4, neighbour);
				}
			}
			else if (vertexMatchesConditionsForS5(graph, neighbour)) {
				removeElementFromStack(s4, neighbour);
				if (!isVertexInStack(s5,neighbour)) {
					pushStack(s5, neighbour);
				}
			}
		}
		// printf("Out\n");
		// printf("Inside s4 after : ");
		// for (int i = 0; i < s4->size; ++i)
		// {
		// 	printf("%d ",s4->vertices[i]->label );
		// }
		// printf("\n");

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

void mergeVertices(GRAPHE *graph, Stack *s4, Stack *s5, Stack *sd) {
	assert(!isStackEmpty(s5));

	SOMMET *v = popStack(s5);
	// Remove v from the graph
	supprimerSommet(graph, v);
	pushStack(sd, v);

	SOMMET *v1 = v->adj->vertex;
	SOMMET *v3 = v->adj->suivant->suivant->vertex;

	ELTADJ *adj1 = v1->adj;
	ELTADJ *adj2 = v3->adj;
	size_t sizeAdj1 = v1->adjSize;
	size_t sizeAdj2 = v3->adjSize;


	// Transfer all the neighbours from one vertex to the other
	ELTADJ *adj = v3->adj;
	for (size_t i = 0; i < sizeAdj2; i++) {

		SOMMET *neighbour = adj->vertex;

		ELTADJ *prec = neighbour->adj;
		ELTADJ *adjNeighbour = neighbour->adj->suivant;
		for (size_t j = 0; j < neighbour->adjSize; j++) {
			if (adjNeighbour->vertex == v1) {
				// Remove duplicate edge from the circular list
				prec->suivant = adjNeighbour->suivant;
//				v3->adjSize--;
				neighbour->adjSize--;
				neighbour->degree--;
				adjNeighbour = adjNeighbour->suivant;

				continue;
			}
			if (adjNeighbour->vertex == v3) {
				adjNeighbour->vertex = v1;
			}
			prec = prec->suivant;
			adjNeighbour = adjNeighbour->suivant;
		}

		adj = adj->suivant;
	}

	// Find v inside the 2 circular lists
	ELTADJ *pl1_prec = adj1;
	ELTADJ *pl1_act = adj1->suivant;
	for (size_t i = 0; i < sizeAdj1; i++) {
		if (pl1_act->vertex == v) {
			break;
		}
		pl1_prec = pl1_act;
		pl1_act = pl1_act->suivant;
	}

	//		------ Looking inside L2
	ELTADJ *pl2_prec = adj2;
	ELTADJ *pl2_act = adj2->suivant;
	for (size_t i = 0; i < sizeAdj2; i++) {
		if (pl2_act->vertex == v) {
			break;
		}
		pl2_prec = pl2_act;
		pl2_act = pl2_act->suivant;
	}

	// Update vertices caused by removing v from the graph
	adj = v->adj;
	for (size_t i = 0; i < v->adjSize; i++) {
		if (vertexMatchesConditionsForS4(adj->vertex)) {
			removeElementFromStack(s5, adj->vertex);
			if (!isVertexInStack(s4, adj->vertex)) {
				pushStack(s4, adj->vertex);
			}
		} else if (vertexMatchesConditionsForS5(graph, adj->vertex)) {
			removeElementFromStack(s4, adj->vertex);
			if (!isVertexInStack(s5, adj->vertex)) {
				pushStack(s5, adj->vertex);
			}
		}
		adj = adj->suivant;
	}

	// Splice the 2 lists
	pl1_prec->suivant = pl2_act->suivant;
	pl2_prec->suivant = pl1_act->suivant;

	// Skip duplicate edge (sometimes the 2 vertices share the same neighbour)
	bool skipDuplicateEdge = false;
	if (pl1_prec->vertex == pl1_prec->suivant->vertex) {
		pl1_prec->suivant = pl1_prec->suivant->suivant;
		skipDuplicateEdge = true;
	}

	// Reset circular list head if we happen to remove the element it points to
	if (v1->adj->vertex == v) {
		v1->adj = pl1_prec->suivant;
	}

	// Minus 2 because we have deleted v twice from both lists
	v1->adjSize = sizeAdj1 + sizeAdj2 - 2;
	if (skipDuplicateEdge) {
		v1->adjSize--;
	}
	v1->degree = v1->adjSize;

	////////
	adj = v->adj;
	for (size_t i = 0; i < v->adjSize; i++) {
		printf("Label: %d, degree: %d\n",
		       adj->vertex->label, adj->vertex->degree);

		adj = adj->suivant;
	}
	////////

//	v1 = v->adj->vertex;
//	v3 = v->adj->suivant->suivant->vertex;

	// if v1 is NOT adjacent to v3
	if (!areNeighbours(v1, v3)) {
//		spliceLists(v1, v3, v);
		supprimerSommet(graph, v3);
		removeElementFromStack(s4, v3);
		removeElementFromStack(s5, v3);
		pushStack(sd, v3);
		v3->mergedWith = v1;
//		supprimerSommet(graph, v1);

		if (vertexMatchesConditionsForS4(v1)) {
			removeElementFromStack(s5, v1);
			if (!isVertexInStack(s4, v1)) {
				pushStack(s4, v1);
			}
		} else if (vertexMatchesConditionsForS5(graph, v1)) {
			removeElementFromStack(s4, v1);
			if (!isVertexInStack(s5, v1)) {
				pushStack(s5, v1);
			}
		} else {
			removeElementFromStack(s4, v1);
			removeElementFromStack(s5, v1);
		}

		// Update one again after the merge
		adj = v1->adj;
		for (size_t i = 0; i < v1->adjSize; i++) {
			if (vertexMatchesConditionsForS4(adj->vertex)) {
				removeElementFromStack(s5, adj->vertex);
				if (!isVertexInStack(s4, adj->vertex)) {
					pushStack(s4, adj->vertex);
				}
			} else if (vertexMatchesConditionsForS5(graph, adj->vertex)) {
				removeElementFromStack(s4, adj->vertex);
				if (!isVertexInStack(s5, adj->vertex)) {
					pushStack(s5, adj->vertex);
				}
			}
			adj = adj->suivant;
		}

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