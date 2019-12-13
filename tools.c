#include "tools.h"

/*
 * Checks whether 2 vertices are neighbours
 */
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

bool vertexMatchesConditionsForS4(SOMMET *vertex) {
	// Degree at most 4
	return vertex->degree <= 4;
}

bool vertexMatchesConditionsForS5(SOMMET *vertex) {
	// Degree five
	if (vertex->degree == 5) {
		// and at least one adjacent vertex with degree at most six
		ELTADJ *adj = vertex->adj;
		for (size_t i = 0;
		     adj != NULL && i < vertex->adjSize; i++, adj = adj->suivant) {
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
		} else if (vertexMatchesConditionsForS5(v)) {
			pushStack(s5, v);
		}
	}
}

void stack4ToStackD(GRAPHE *graph, Stack *s4, Stack *sd, Stack *s5) {
	SOMMET *v;
	while (!isStackEmpty(s4)) {
		// Transfer v from s4 to sd
		v = popStack(s4);
		supprimerSommet(graph, v);
		pushStack(sd, v);

		// Check the neighbours of v and update the stacks
		ELTADJ *padj = v->adj;
		for (size_t i = 0;
		     padj != NULL && i < v->adjSize; i++, padj = padj->suivant) {
			SOMMET *neighbour = padj->vertex;
			if (vertexMatchesConditionsForS4(neighbour)) {
				removeElementFromStack(s5, neighbour);
				if (!isVertexInStack(s4, neighbour)) {
					pushStack(s4, neighbour);
				}
			} else if (vertexMatchesConditionsForS5(neighbour)) {
				removeElementFromStack(s4, neighbour);
				if (!isVertexInStack(s5, neighbour)) {
					pushStack(s5, neighbour);
				}
			}
		}
	}
}

void mergeVertices(GRAPHE *graph, Stack *s4, Stack *s5, Stack *sd) {
	assert(!isStackEmpty(s5));

	SOMMET *v = popStack(s5);
	// Remove v from the graph
	supprimerSommet(graph, v);
	pushStack(sd, v);

	SOMMET *v1 = v->adj->vertex;
	SOMMET *v2 = v->adj->suivant->vertex;
	SOMMET *v3 = v->adj->suivant->suivant->vertex;
	SOMMET *v4 = v->adj->suivant->suivant->suivant->vertex;

	SOMMET *mergingTo, *mergingFrom;

	if (!areNeighbours(v1, v3)) {
		mergingTo = v1;
		mergingFrom = v3;
	} else {
		mergingTo = v2;
		mergingFrom = v4;
	}

	// Do the merging
	ELTADJ *adj1 = mergingTo->adj;
	ELTADJ *adj2 = mergingFrom->adj;
	size_t sizeAdj1 = mergingTo->adjSize;
	size_t sizeAdj2 = mergingFrom->adjSize;

	// Transfer all the neighbours from one vertex to the other
	ELTADJ *adj = mergingFrom->adj;
	for (size_t i = 0; i < sizeAdj2; i++) {
		SOMMET *neighbour = adj->vertex;

		ELTADJ *prec = neighbour->adj;
		ELTADJ *adjNeighbour = neighbour->adj->suivant;
		for (size_t j = 0; j < neighbour->adjSize; j++) {
			if (adjNeighbour->vertex == mergingTo) {
				// Remove duplicate edge from the circular list
				prec->suivant = adjNeighbour->suivant;
				neighbour->adjSize--;
				neighbour->degree--;
				adjNeighbour = adjNeighbour->suivant;

				continue;
			}
			if (adjNeighbour->vertex == mergingFrom) {
				adjNeighbour->vertex = mergingTo;
			}
			prec = prec->suivant;
			adjNeighbour = adjNeighbour->suivant;
		}

		adj = adj->suivant;
	}

	// Splice the adjacency lists

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
		} else if (vertexMatchesConditionsForS5(adj->vertex)) {
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
	if (mergingTo->adj->vertex == v) {
		mergingTo->adj = pl1_prec->suivant;
	}

	// Minus 2 because we have deleted v twice from both lists
	mergingTo->adjSize = sizeAdj1 + sizeAdj2 - 2;
	if (skipDuplicateEdge) {
		mergingTo->adjSize--;
	}
	mergingTo->degree = mergingTo->adjSize;

	supprimerSommet(graph, mergingFrom);
	removeElementFromStack(s4, mergingFrom);
	removeElementFromStack(s5, mergingFrom);
	pushStack(sd, mergingFrom);
	mergingFrom->mergedWith = mergingTo;

	if (vertexMatchesConditionsForS4(mergingTo)) {
		removeElementFromStack(s5, mergingTo);
		if (!isVertexInStack(s4, mergingTo)) {
			pushStack(s4, mergingTo);
		}
	} else if (vertexMatchesConditionsForS5(mergingTo)) {
		removeElementFromStack(s4, mergingTo);
		if (!isVertexInStack(s5, mergingTo)) {
			pushStack(s5, mergingTo);
		}
	} else {
		removeElementFromStack(s4, mergingTo);
		removeElementFromStack(s5, mergingTo);
	}

	// Update the stacks one again after the merge
	adj = mergingTo->adj;
	for (size_t i = 0; i < mergingTo->adjSize; i++) {
		if (vertexMatchesConditionsForS4(adj->vertex)) {
			removeElementFromStack(s5, adj->vertex);
			if (!isVertexInStack(s4, adj->vertex)) {
				pushStack(s4, adj->vertex);
			}
		} else if (vertexMatchesConditionsForS5(adj->vertex)) {
			removeElementFromStack(s4, adj->vertex);
			if (!isVertexInStack(s5, adj->vertex)) {
				pushStack(s5, adj->vertex);
			}
		}
		adj = adj->suivant;
	}
}


Color getColorFromArray(Color *colors) {
	assert(colors != NULL);
	for (int i = 0; i < 5; ++i) {
		if ((int) colors[i] >= 0) {
			return colors[i];
		}
	}
	return colors[0];
}
