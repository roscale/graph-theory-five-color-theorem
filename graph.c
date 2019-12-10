#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "graph.h"

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
		pointeur->info = -1;
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
	SOMMET *psommet2 = b;

	// We take the negative in order to inverse the order (clockwise)
	double relative_angle = -atan2(
		  b->position.y - a->position.y,
		  b->position.x - a->position.x);

	ELTADJ *padj = psommet->adj, *precedent;

	if (padj == NULL) /* la liste d'adjacence est vide */
	{
		padj = (ELTADJ *) malloc(sizeof(ELTADJ));
		if (padj == NULL) {
			printf(
				  "Erreur! Memoire insuffisante pour creer un sommet\n");
			return -3;
		} else {
			psommet->adj = padj; /* premier element de la liste d'adjacence */
			padj->suivant = NULL;
		}
	} else /* la liste d'adjacence est non vide, on la parcourt pour voir si b s'y trouve */
	{
		if (padj->relative_angle > relative_angle) {
			padj = (ELTADJ *) malloc(sizeof(ELTADJ));
			if (padj == NULL) {
				printf(
					  "Erreur! Memoire insuffisante pour creer un sommet\n");
				return -3;
			} else {
				padj->suivant = psommet->adj;
				psommet->adj = padj;
			}
		} else {
			while (padj != NULL) {
				if (padj->relative_angle > relative_angle) {
					padj = NULL;
					break;
				} /* on depasse b sans le trouver */
				precedent = padj;
				padj = padj->suivant;
			}
			if (padj == NULL) /* l'arc n'existe pas, il faut le creer */
			{
				padj = (ELTADJ *) malloc(sizeof(ELTADJ));
				if (padj == NULL) {
					printf(
						  "Erreur! Memoire insuffisante pour creer un sommet\n");
					return -3;
				} else if (precedent->suivant ==
				           NULL) /* element ajouter a la fin */
				{
					precedent->suivant = padj;
					padj->suivant = NULL;
				} else /* element ajouter "au milieu" pour garder ordre */
				{
					padj->suivant = precedent->suivant;
					precedent->suivant = padj;
				}
			}
		}
	}

	padj->vertex = b;
	padj->relative_angle = relative_angle;
	g->nbA++;
	psommet->degree++;
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
	ELTADJ *padj, *suivant, *precedent_adj;
	int flag_premier_sommet, flag_premier_arc;
	if (g->premierSommet == NULL) {
		printf("Erreur! Graphe vide, suppression impossible\n");
		return -1;
	} else {
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
		if (psommet == NULL) {
			printf("Erreur! Le sommet a supprimer n'existe pas\n");
			return -1;
		} else {
			if (psommet->suivant == NULL) g->dernierSommet = precedent;

			if (flag_premier_sommet == 1) g->premierSommet = psommet->suivant;
			else precedent->suivant = psommet->suivant;
			padj = psommet->adj;
			// free(psommet);
			g->nbS--;
			while (padj != NULL) {
				suivant = padj->suivant;
				// free(padj);
				g->nbA--;
				padj = suivant;
			}
		}

		/* il faut aussi supprimer les arcs ayant le sommet a supprimer comme extremite */
		psommet = g->premierSommet;
		while (psommet != NULL) {
			padj = psommet->adj;
			flag_premier_arc = 1;
			while (padj != NULL) {
				if (padj->vertex == a) break;
				else {
					flag_premier_arc = 0;
					precedent_adj = padj;
					padj = padj->suivant;
				}
			}
			if (padj != NULL) {
				if (flag_premier_arc == 1) psommet->adj = padj->suivant;
				else precedent_adj->suivant = padj->suivant;
				// free(padj);
				psommet->degree--;
				g->nbA--;
			}
			psommet = psommet->suivant;
		}
		return 0;
	}
}

int supprimerArc(GRAPHE *g, SOMMET *a, SOMMET *b) {
	SOMMET *psommet = a;
	ELTADJ *padj, *precedent_adj;
	int flag_premier_arc;
	padj = psommet->adj;
	flag_premier_arc = 1;
	while (padj != NULL) {
		if (padj->vertex == b) break;
		else {
			flag_premier_arc = 0;
			precedent_adj = padj;
			padj = padj->suivant;
		}
	}
	if (padj != NULL) {
		if (flag_premier_arc == 1) psommet->adj = padj->suivant;
		else precedent_adj->suivant = padj->suivant;
		free(padj);
		g->nbA--;
	} else {
		printf(
			  "Erreur! L'extremite de l'arc a supprimer n'existe pas\n");
		return -1;
	}
	psommet->degree--;
	return 0;
}

bool removeEdge(GRAPHE *g, SOMMET *a, SOMMET *b) {
	if (supprimerArc(g, a, b) != 0) {
		return false;
	}
	if (supprimerArc(g, b, a) != 0) {
		return false;
	}
	return true;
}

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
		case RED: return "RED";
		case GREEN: return "GREEN";
		case BLUE: return "BLUE";
		case YELLOW: return "YELLOW";
		case PURPLE: return "PURPLE";
		// default: assert(false && "Invalid color");
		default: return "Unknown color";
	}
}

void afficherGraphe(GRAPHE *g) {
	SOMMET *psommet;
	ELTADJ *padj;
	if (g->premierSommet == NULL) printf("graphe vide\n");
	else {
		printf("nbS=%d , nbA=%d, label max.=%d\n", g->nbS, g->nbA, g->maxS);
		psommet = g->premierSommet;
		do {
			printf("\n");
			printf("Sommet de label: %d, degre: %d et couleur: %s\n",
				  psommet->label, psommet->degree, colorToString(psommet->info));
			if (psommet->adj == NULL)
				printf(" -> ce sommet n'a aucun arc sortant\n ");
			else {
				padj = psommet->adj;
				do {
					printf(" -> arc de %d vers %d, couleur: %s\n",
					       psommet->label, padj->vertex->label, colorToString(padj->vertex->info));
					padj = padj->suivant;
				} while (padj != NULL);
			}
			printf("\n");
			psommet = psommet->suivant;
		} while (psommet != NULL);
	}
}
