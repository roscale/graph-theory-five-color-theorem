#include "tools.h"

// Store pointers to all the heap allocated memory
static void *pointers[1000];
static size_t nPointers = 0;

void *allocateMemory(size_t size) {
	void *p = malloc(size);
	if (p != NULL) {
		pointers[nPointers++] = p;
		return p;
	} else {
		freeMemory();
		fprintf(stderr, "Memory allocation error\n");
		exit(1);
	}
}

void freeMemory() {
	for (size_t i = 0; i < nPointers; i++) {
		free(pointers[i]);
	}
}

/*
 * Checks whether 2 vertices are neighbours
 */
static bool areNeighbours(Vertex *v1, Vertex *v2) {
	AdjList *adj = v1->adj;
	for (size_t i = 0; i < v1->adjSize; i++) {
		if (adj->vertex == v2) {
			return true;
		}
		adj = adj->next;
	}
	return false;
}

bool vertexMatchesConditionsForS4(Vertex *vertex) {
	// Degree at most 4
	return vertex->degree <= 4;
}

bool vertexMatchesConditionsForS5(Vertex *vertex) {
	// Degree five
	if (vertex->degree == 5) {
		// and at least one adjacent vertex with degree at most six
		AdjList *adj = vertex->adj;
		for (size_t i = 0;
		     adj != NULL && i < vertex->adjSize; i++, adj = adj->next) {
			if (adj->vertex->degree <= 6) {
				return true;
			}
		}
	}
	return false;
}

void populateStacks(Graph *graph, Stack *s4, Stack *s5) {
	for (Vertex *v = graph->firstVertex; v != NULL; v = v->next) {
		if (vertexMatchesConditionsForS4(v)) {
			pushStack(s4, v);
		} else if (vertexMatchesConditionsForS5(v)) {
			pushStack(s5, v);
		}
	}
}

void stack4ToStackD(Graph *graph, Stack *s4, Stack *sd, Stack *s5) {
	Vertex *v;
	while (!isStackEmpty(s4)) {
		// Transfer v from s4 to sd
		v = popStack(s4);
		removeVertex(graph, v);
		pushStack(sd, v);

		// Check the neighbours of v and update the stacks
		AdjList *adj = v->adj;
		for (size_t i = 0;
		     adj != NULL && i < v->adjSize; i++, adj = adj->next) {
			Vertex *neighbour = adj->vertex;
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

void mergeVertices(Graph *graph, Stack *s4, Stack *s5, Stack *sd) {
	assert(!isStackEmpty(s5));

	Vertex *v = popStack(s5);
	// Remove v from the graph
	removeVertex(graph, v);
	pushStack(sd, v);

	Vertex *v1 = v->adj->vertex;
	Vertex *v2 = v->adj->next->vertex;
	Vertex *v3 = v->adj->next->next->vertex;
	Vertex *v4 = v->adj->next->next->next->vertex;

	Vertex *mergingTo, *mergingFrom;

	if (!areNeighbours(v1, v3)) {
		mergingTo = v1;
		mergingFrom = v3;
	} else {
		mergingTo = v2;
		mergingFrom = v4;
	}

	// Do the merging
	AdjList *adjTo = mergingTo->adj;
	AdjList *adjFrom = mergingFrom->adj;
	size_t sizeAdjTo = mergingTo->adjSize;
	size_t sizeAdjFrom = mergingFrom->adjSize;

	// Transfer all the neighbours from one vertex to the other
	for (size_t i = 0; i < sizeAdjFrom; i++) {
		Vertex *neighbour = adjFrom->vertex;

		AdjList *previous = neighbour->adj;
		AdjList *adjNeighbour = neighbour->adj->next;
		for (size_t j = 0; j < neighbour->adjSize; j++) {
			if (adjNeighbour->vertex == mergingTo) {
				// Remove duplicate edge from the circular list
				previous->next = adjNeighbour->next;
				neighbour->adjSize--;
				neighbour->degree--;
				adjNeighbour = adjNeighbour->next;

				continue;
			}
			if (adjNeighbour->vertex == mergingFrom) {
				adjNeighbour->vertex = mergingTo;
			}
			previous = previous->next;
			adjNeighbour = adjNeighbour->next;
		}

		adjFrom = adjFrom->next;
	}

	// Splice the adjacency lists

	// Find v inside the 2 circular lists
	AdjList *pl1_prec = adjTo;
	AdjList *pl1_act = adjTo->next;
	for (size_t i = 0; i < sizeAdjTo; i++) {
		if (pl1_act->vertex == v) {
			break;
		}
		pl1_prec = pl1_act;
		pl1_act = pl1_act->next;
	}

	//		------ Looking inside L2
	AdjList *pl2_prec = adjFrom;
	AdjList *pl2_act = adjFrom->next;
	for (size_t i = 0; i < sizeAdjFrom; i++) {
		if (pl2_act->vertex == v) {
			break;
		}
		pl2_prec = pl2_act;
		pl2_act = pl2_act->next;
	}

	// Update vertices caused by removing v from the graph
	AdjList *adj = v->adj;
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
		adj = adj->next;
	}

	// Splice the 2 lists
	pl1_prec->next = pl2_act->next;
	pl2_prec->next = pl1_act->next;

	// Skip duplicate edge (sometimes the 2 vertices share the same neighbour)
	bool skipDuplicateEdge = false;
	if (pl1_prec->vertex == pl1_prec->next->vertex) {
		pl1_prec->next = pl1_prec->next->next;
		skipDuplicateEdge = true;
	}

	// Reset circular list head if we happen to remove the element it points to
	if (mergingTo->adj->vertex == v) {
		mergingTo->adj = pl1_prec->next;
	}

	// Minus 2 because we have deleted v twice from both lists
	mergingTo->adjSize = sizeAdjTo + sizeAdjFrom - 2;
	if (skipDuplicateEdge) {
		mergingTo->adjSize--;
	}
	mergingTo->degree = mergingTo->adjSize;

	removeVertex(graph, mergingFrom);
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
		adj = adj->next;
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
