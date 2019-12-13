#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "graph.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void initialiserGraphe(GRAPHE *g) {
	g->nbS = 0;
	g->nbA = 0;
	g->maxS = 0;
	g->premierSommet = NULL;
	g->dernierSommet = NULL;
}

SOMMET *ajouterSommet(GRAPHE *g, Position position) {
	SOMMET *pointeur;
	g->maxS++;

	// Allocate memory for the vertex
	pointeur = (SOMMET *) malloc(sizeof(SOMMET));
	if (pointeur == NULL) {
		printf("Erreur! Memoire insuffisante pour creer un sommet\n");
		return NULL;
	} else {
		pointeur->label = g->maxS;
		pointeur->position = position;
		pointeur->degree = 0;
		pointeur->suivant = NULL;
		pointeur->adj = NULL;
		pointeur->adjSize = 0;
		pointeur->info = -1;
		pointeur->mergedWith = NULL;

		if (g->nbS == 0) {
			g->premierSommet = pointeur;
			g->dernierSommet = pointeur;
		} else {
			g->dernierSommet->suivant = pointeur;
			g->dernierSommet = pointeur;
		}
		g->nbS++;
		return pointeur;
	}
}

int ajouterArc(GRAPHE *g, SOMMET *a, SOMMET *b) {
	SOMMET *psommet = a;

	// We take the opposite in order to inverse the order
	// clockwise instead of counterclockwise
	double relative_angle = -atan2(
		  b->position.y - a->position.y,
		  b->position.x - a->position.x);

	ELTADJ *padj = (ELTADJ *) malloc(sizeof(ELTADJ));
	if (padj == NULL) {
		printf("Erreur! Memoire insuffisante pour creer un sommet\n");
		return -3;
	}

	// The adjacency list is empty
	if (psommet->adj == NULL) {
		psommet->adjSize = 0;
		psommet->adj = padj;
		padj->suivant = padj;

	} else if (psommet->adjSize == 1) {
		// Only one element, doesn't matter where we add it
		psommet->adj->suivant = padj;
		padj->suivant = psommet->adj;

		// Begin the circular list with a vertex degree at most 6
		if (b->degree <= 6) {
			psommet->adj = padj;
		}

	} else if (psommet->adjSize >= 2) {
		ELTADJ *before = psommet->adj;
		ELTADJ *after = psommet->adj->suivant;

		// Iterate the list
		for (size_t i = 0; i < psommet->adjSize; i++) {
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
				padj->suivant = after;
				before->suivant = padj;
				break;
			}

			before = before->suivant;
			after = after->suivant;
		}
	}

	padj->vertex = b;
	padj->relative_angle = relative_angle;
	g->nbA++;
	psommet->degree++;
	psommet->adjSize++;
	return 0;
}

bool addEdge(GRAPHE *g, SOMMET *a, SOMMET *b) {
	if (ajouterArc(g, a, b) != 0) {
		return false;
	}
	if (ajouterArc(g, b, a) != 0) {
		return false;
	}
	return true;
}

int supprimerSommet(GRAPHE *g, SOMMET *a) {
	SOMMET *psommet, *precedent;
	int flag_premier_sommet;

	// The graph is empty
	if (g->premierSommet == NULL) {
		printf("Erreur! Graphe vide, suppression impossible\n");
		return -1;
	} else {
		// We search the vertex in the graph
		psommet = g->premierSommet;
		flag_premier_sommet = 1;
		while (psommet != NULL) {
			if (psommet == a) break;
			else {
				flag_premier_sommet = 0;
				precedent = psommet;
				psommet = psommet->suivant;
			}
		}
		// The vertex doesn't exist in the graph
		if (psommet == NULL) {
			printf("Erreur! Le sommet a supprimer n'existe pas\n");
			return -1;
		} else {
			// Remove the vertex from the graph
			if (psommet->suivant == NULL) g->dernierSommet = precedent;

			if (flag_premier_sommet == 1) g->premierSommet = psommet->suivant;
			else precedent->suivant = psommet->suivant;
			g->nbS--;
		}

		g->nbA -= psommet->adjSize;

		// Also remove any links between the neighbours of the vertex
		psommet = g->premierSommet;
		while (psommet != NULL) {
			supprimerArc(psommet, a);
			psommet = psommet->suivant;
		}
		return 0;
	}
}

int supprimerArc(SOMMET *a, SOMMET *b) {
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
		ELTADJ *previous = a->adj;
		ELTADJ *current = a->adj->suivant;

		bool found = false;
		for (size_t i = 0; i < a->adjSize; i++) {
			if (current->vertex == b) {
				previous->suivant = current->suivant;

				// If we remove the head of the list
				if (a->adj == current) {
					a->adj = previous;
				}
				a->adjSize--;
				a->degree--;

				found = true;
				break;
			}

			previous = previous->suivant;
			current = current->suivant;
		}

		if (!found) {
			return -1;
		}
	}

	return 0;
}

// DO NOT USE
// TODO Fix freeing of the circular adjacency list
void supprimerGraphe(GRAPHE *g) {
	SOMMET *psommet, *temps;
	ELTADJ *padj, *tempadj;
	psommet = g->premierSommet;
	while (psommet != NULL) {
		padj = psommet->adj;
		while (padj != NULL) {
			tempadj = padj;
			padj = padj->suivant;
			free(tempadj);
		}
		temps = psommet;
		psommet = psommet->suivant;
		free(temps);
	}
	initialiserGraphe(g);
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

void afficherGraphe(GRAPHE *g) {
	SOMMET *psommet;
	if (g->premierSommet == NULL) printf("graphe vide\n");
	else {
		printf("nbS=%d , nbA=%d, label max.=%d\n", g->nbS, g->nbA, g->maxS);
		psommet = g->premierSommet;

		do {
			// Print this vertex
			printf("\nSommet de label: %d, degre: %d et couleur: %s\n",
			       psommet->label, psommet->degree,
			       colorToString(psommet->info));

			if (psommet->adj == NULL)
				printf(" -> ce sommet n'a aucun arc sortant\n\n");

			// Print neighbours
			ELTADJ *adj = psommet->adj;
			for (size_t i = 0; i < psommet->adjSize; i++) {
				printf(" -> arc de %d vers %d, couleur: %s\n",
				       psommet->label, adj->vertex->label,
				       colorToString(adj->vertex->info));

				adj = adj->suivant;
			}

			psommet = psommet->suivant;
		} while (psommet != NULL);
	}
}
