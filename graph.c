#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "graph.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void initializeGraph(Graph *g) {
	g->nVertices = 0;
	g->nArcs = 0;
	g->maxVertices = 0;
	g->firstVertex = NULL;
	g->lastVertex = NULL;
}

Vertex *addVertex(Graph *g, Position position) {
	Vertex *v;
	g->maxVertices++;

	// Allocate memory for the vertex
	v = (Vertex *) malloc(sizeof(Vertex));
	if (v == NULL) {
		printf("Erreur! Memoire insuffisante pour creer un sommet\n");
		return NULL;
	} else {
		v->label = g->maxVertices;
		v->position = position;
		v->degree = 0;
		v->next = NULL;
		v->adj = NULL;
		v->adjSize = 0;
		v->color = -1;
		v->mergedWith = NULL;

		if (g->nVertices == 0) {
			g->firstVertex = v;
			g->lastVertex = v;
		} else {
			g->lastVertex->next = v;
			g->lastVertex = v;
		}
		g->nVertices++;
		return v;
	}
}

int addArc(Graph *g, Vertex *a, Vertex *b) {
	// We take the opposite in order to inverse the order
	// clockwise instead of counterclockwise
	double relative_angle = -atan2(
		  b->position.y - a->position.y,
		  b->position.x - a->position.x);

	AdjList *adj = (AdjList *) malloc(sizeof(AdjList));
	if (adj == NULL) {
		printf("Erreur! Memoire insuffisante pour creer un sommet\n");
		return -3;
	}

	// The adjacency list is empty
	if (a->adj == NULL) {
		a->adjSize = 0;
		a->adj = adj;
		adj->next = adj;

	} else if (a->adjSize == 1) {
		// Only one element, doesn't matter where we add it
		a->adj->next = adj;
		adj->next = a->adj;

		// Begin the circular list with a vertex degree at most 6
		if (b->degree <= 6) {
			a->adj = adj;
		}

	} else if (a->adjSize >= 2) {
		AdjList *before = a->adj;
		AdjList *after = a->adj->next;

		// Iterate the list
		for (size_t i = 0; i < a->adjSize; i++) {
			double beforeAngle = before->relative_angle;
			double afterAngle = after->relative_angle;

			// Keep the circular increasing order between the angles
			if (beforeAngle > afterAngle) {
				// Do a full cycle to restore the order
				// Compare positives with positives and negatives with negatives
				if (relative_angle >= 0) {
					afterAngle += 2 * M_PI;
				} else {
					beforeAngle -= 2 * M_PI;
				}
			}

			if (beforeAngle <= relative_angle &&
			    relative_angle <= afterAngle) {

				// Insert it in-between
				adj->next = after;
				before->next = adj;
				break;
			}

			before = before->next;
			after = after->next;
		}
	}

	adj->vertex = b;
	adj->relative_angle = relative_angle;
	g->nArcs++;
	a->degree++;
	a->adjSize++;
	return 0;
}

bool addEdge(Graph *g, Vertex *a, Vertex *b) {
	if (addArc(g, a, b) != 0) {
		return false;
	}
	if (addArc(g, b, a) != 0) {
		return false;
	}
	return true;
}

int removeVertex(Graph *g, Vertex *a) {
	Vertex *current, *previous;
	int flagFirstVertex;

	// The graph is empty
	if (g->firstVertex == NULL) {
		printf("Erreur! Graphe vide, suppression impossible\n");
		return -1;
	} else {
		// We search the vertex in the graph
		current = g->firstVertex;
		flagFirstVertex = 1;
		while (current != NULL) {
			if (current == a) break;
			else {
				flagFirstVertex = 0;
				previous = current;
				current = current->next;
			}
		}
		// The vertex doesn't exist in the graph
		if (current == NULL) {
			printf("Erreur! Le sommet a supprimer n'existe pas\n");
			return -1;
		} else {
			// Remove the vertex from the graph
			if (current->next == NULL) g->lastVertex = previous;

			if (flagFirstVertex == 1) g->firstVertex = current->next;
			else previous->next = current->next;
			g->nVertices--;
		}

		g->nArcs -= current->adjSize;

		// Also remove any links between the neighbours of the vertex
		current = g->firstVertex;
		while (current != NULL) {
			removeArc(current, a);
			current = current->next;
		}
		return 0;
	}
}

int removeArc(Vertex *a, Vertex *b) {
	// No elements in the adjacency list
	if (a->adj == NULL) {
		return -1;
	}

		// One element left
	else if (a->adjSize == 1) {
		if (a->adj->vertex == b) {
			free(a->adj);
			a->adj = NULL;
			a->adjSize--;
			a->degree--;
		}
	}

		// Multiple elements left
	else if (a->adjSize >= 2) {
		// Iterate the list with 2 pointers concurrently
		AdjList *previous = a->adj;
		AdjList *current = a->adj->next;

		bool found = false;
		for (size_t i = 0; i < a->adjSize; i++) {
			if (current->vertex == b) {
				previous->next = current->next;

				// If we remove the head of the list
				if (a->adj == current) {
					a->adj = previous;
				}
				a->adjSize--;
				a->degree--;

				found = true;
				break;
			}

			previous = previous->next;
			current = current->next;
		}

		if (!found) {
			return -1;
		}
	}

	return 0;
}

// DO NOT USE
// TODO Fix freeing of the circular adjacency list
void supprimerGraphe(Graph *g) {
	Vertex *psommet, *temps;
	AdjList *padj, *tempadj;
	psommet = g->firstVertex;
	while (psommet != NULL) {
		padj = psommet->adj;
		while (padj != NULL) {
			tempadj = padj;
			padj = padj->next;
			free(tempadj);
		}
		temps = psommet;
		psommet = psommet->next;
		free(temps);
	}
	initializeGraph(g);
}

const char *colorToString(Color color) {
	switch (color) {
		case RED:
			return "RED";
		case GREEN:
			return "GREEN";
		case BLUE:
			return "BLUE";
		case YELLOW:
			return "YELLOW";
		case PURPLE:
			return "PURPLE";
		default:
			return "Unknown color";
	}
}

void printGraph(Graph *g) {
	Vertex *vertex;
	if (g->firstVertex == NULL) printf("graphe vide\n");
	else {
		printf("nbS=%d , nbA=%d, label max.=%d\n", g->nVertices, g->nArcs, g->maxVertices);
		vertex = g->firstVertex;

		do {
			// Print this vertex
			printf("\nSommet de label: %d, degre: %d et couleur: %s\n",
			       vertex->label, vertex->degree,
			       colorToString(vertex->color));

			if (vertex->adj == NULL)
				printf(" -> ce sommet n'a aucun arc sortant\n\n");

			// Print neighbours
			AdjList *adj = vertex->adj;
			for (size_t i = 0; i < vertex->adjSize; i++) {
				printf(" -> arc de %d vers %d, couleur: %s\n",
				       vertex->label, adj->vertex->label,
				       colorToString(adj->vertex->color));

				adj = adj->next;
			}

			vertex = vertex->next;
		} while (vertex != NULL);
	}
}
