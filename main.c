#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "Stack.h"
#include "graph.h"
#include "tools.h"

// https://en.wikipedia.org/wiki/Five_color_theorem?fbclid=IwAR2gWXQwMi2bhCh9HIW6gnY_vz_qEf5Cwk5j-c-YYZk23LTf-KPZSMsBNJY


int main() {
	Stack *s4 = createStack(100);
	Stack *s5 = createStack(100);
	Stack *sd = createStack(100);
	Stack *sTrash = createStack(100);

	// Graph example 1
	GRAPHE *graph = malloc(sizeof(GRAPHE));
	initialiserGraphe(graph);

	// TODO Give each vertex a position
	Position placeholder = {0, 0};

	// We receive pointers instead of labels
	SOMMET *a = ajouterSommet(graph, placeholder);
	SOMMET *b = ajouterSommet(graph, placeholder);
	SOMMET *c = ajouterSommet(graph, placeholder);
	SOMMET *d = ajouterSommet(graph, placeholder);
	SOMMET *e = ajouterSommet(graph, placeholder);
	SOMMET *f = ajouterSommet(graph, placeholder);

	// Returns 0 on success
	addEdge(graph, a, b);
	addEdge(graph, a, c);
	addEdge(graph, b, c);

	addEdge(graph, a, f);
	addEdge(graph, a, e);
	addEdge(graph, a, d);
	addEdge(graph, c, d);
	addEdge(graph, b, d);
	addEdge(graph, d, f);
	addEdge(graph, e, f);

	// Graph exemple 2 ( 5-complet )
	// https://i.stack.imgur.com/rO3SR.png
	// It is a planar graph with a minimum degree of 5
	GRAPHE *graph2 = malloc(sizeof(GRAPHE));
	initialiserGraphe(graph2);

	SOMMET *a2 = ajouterSommet(graph2, placeholder);
	SOMMET *b2 = ajouterSommet(graph2, placeholder);
	SOMMET *c2 = ajouterSommet(graph2, placeholder);
	SOMMET *d2 = ajouterSommet(graph2, placeholder);
	SOMMET *e2 = ajouterSommet(graph2, placeholder);
	SOMMET *f2 = ajouterSommet(graph2, placeholder);
	SOMMET *g2 = ajouterSommet(graph2, placeholder);
	SOMMET *h2 = ajouterSommet(graph2, placeholder);
	SOMMET *i2 = ajouterSommet(graph2, placeholder);
	SOMMET *j2 = ajouterSommet(graph2, placeholder);
	SOMMET *k2 = ajouterSommet(graph2, placeholder);
	SOMMET *l2 = ajouterSommet(graph2, placeholder);

	// Returns 0 on success
	addEdge(graph2, a2, b2);
	addEdge(graph2, a2, c2);
	addEdge(graph2, a2, d2);
	addEdge(graph2, a2, i2);
	addEdge(graph2, a2, h2);

	addEdge(graph2, b2, d2);
	addEdge(graph2, b2, e2);
	addEdge(graph2, b2, f2);
	addEdge(graph2, b2, c2);

	addEdge(graph2, c2, f2);
	addEdge(graph2, c2, g2);
	addEdge(graph2, c2, h2);

	addEdge(graph2, i2, j2);
	addEdge(graph2, i2, l2);
	addEdge(graph2, i2, d2);
	addEdge(graph2, i2, h2);

	addEdge(graph2, d2, j2);
	addEdge(graph2, d2, e2);

	addEdge(graph2, e2, j2);
	addEdge(graph2, e2, k2);
	addEdge(graph2, e2, f2);

	addEdge(graph2, f2, k2);
	addEdge(graph2, f2, g2);

	addEdge(graph2, g2, k2);
	addEdge(graph2, g2, l2);
	addEdge(graph2, g2, h2);

	addEdge(graph2, h2, l2);

	addEdge(graph2, l2, j2);
	addEdge(graph2, l2, k2);

	addEdge(graph2, k2, j2);

	printf("Initial graph:\n");
	afficherGraphe(graph);
	// afficherGraphe(graph2);

	// STEP 1
	populateStacks(graph, s4, s5);
	// populateStacks(graph2, s4, s5);

	// STEP 2
	// printf("----------In step2\n");
	// printf("Inside s4 : ");
	// for (int i = 0; i < s4->size; ++i)
	// {
	// 	printf("%d ",s4->vertices[i]->label );
	// }
	// printf("\n");
	// printf("Inside s5 : ");
	// for (int i = 0; i < s5->size; ++i)
	// {
	// 	printf("%d ",s5->vertices[i]->label );
	// }
	// printf("\n");
	// printf("Inside sd : ");
	// for (int i = 0; i < sd->size; ++i)
	// {
	// 	printf("%d ",sd->vertices[i]->label );
	// }
	// printf("\n");

	stack4ToStackD(graph, s4, sd, s5);
	// stack4ToStackD(graph2, s4, sd, s5);
	// printf("----------Out step2\n");
	// printf("Inside s4 : ");
	// for (int i = 0; i < s4->size; ++i)
	// {
	// 	printf("%d ",s4->vertices[i]->label );
	// }
	// printf("\n");
	// printf("Inside s5 : ");
	// for (int i = 0; i < s5->size; ++i)
	// {
	// 	printf("%d ",s5->vertices[i]->label );
	// }
	// printf("\n");
	// printf("Inside sd : ");
	// for (int i = 0; i < sd->size; ++i)
	// {
	// 	printf("%d ",sd->vertices[i]->label );
	// }
	// printf("\n");

	// printf("Graph after step2:\n");
	// afficherGraphe(graph);
	// printf("graph nbs : %d, nba : %d, maxS : %d\n", graph->nbS, graph->nbA, graph->maxS);
	// afficherGraphe(graph2);

	// STEP 3
	assert(isStackEmpty(s4)); // deg(graph) >= 5

	// The graph is empty
	if (graph->nbS == 0) {
		// STEP 5
		
		SOMMET *v;
		while (!isStackEmpty(sd)) {
			v = popStack(sd);
			
			// printf("neighbour of %d colors = [ ", v->label);
			Color c[5] = {0, 1, 2, 3, 4};
			for (ELTADJ *padj = v->adj; padj != NULL; padj = padj->suivant) {
				// printf("Dest : %d, info : %d\n",padj->dest, padj->info);
				// printf("%d ", padj->vertex->info );
				if(padj->vertex->info >= 0 && padj->vertex->info < 5){
					c[padj->vertex->info] = -1;
				}
			}
			// printf("]\n");
			// printf("Colors : ");
			// for (int i = 0; i < 5; ++i)
			// {
			// 	printf("%d",c[i] );
			// }
			// printf("\n");
			v->info = getColorFromArray(c);
			// printf("Color of %d : %d\n", v->label, v->info );
			pushStack(sTrash, v);
		}

		printf("----------- Affichage des couleurs des diff somment : \n");
		while (!isStackEmpty(sTrash)){
			SOMMET *v = popStack(sTrash);
			printf("v(%d):%s ", v->label, colorToString(v->info) );
		}
		printf("\n");
	} else {
		mergeVertices(graph, s5, sd);
		// mergeVertices(graph2, s5, sd);
	}

	

	return 0;
}