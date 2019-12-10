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

	int a = ajouterSommet(graph, -1);
	int b = ajouterSommet(graph, -1);
	int c = ajouterSommet(graph, -1);
	int d = ajouterSommet(graph, -1);
	int e = ajouterSommet(graph, -1);
	int f = ajouterSommet(graph, -1);

	// Returns 0 on success
	addEdge(graph, a, b, 0);
	addEdge(graph, a, c, 0);
	addEdge(graph, b, c, 0);

	addEdge(graph, a, f, 0);
	addEdge(graph, a, e, 0);
	addEdge(graph, a, d, 0);
	addEdge(graph, c, d, 0);
	addEdge(graph, b, d, 0);
	addEdge(graph, d, f, 0);
	addEdge(graph, e, f, 0);

	// Graph exemple 2 ( 5-complet )
	GRAPHE *graph2 = malloc(sizeof(GRAPHE));
	initialiserGraphe(graph2);

	int a2 = ajouterSommet(graph2, 1);
	int b2 = ajouterSommet(graph2, 2);
	int c2 = ajouterSommet(graph2, 3);
	int d2 = ajouterSommet(graph2, 4);
	int e2 = ajouterSommet(graph2, 5);
	int f2 = ajouterSommet(graph2, 6);
	int g2 = ajouterSommet(graph2, 7);
	int h2 = ajouterSommet(graph2, 8);
	int i2 = ajouterSommet(graph2, 9);
	int j2 = ajouterSommet(graph2, 10);
	int k2 = ajouterSommet(graph2, 11);
	int l2 = ajouterSommet(graph2, 12);

	// Returns 0 on success
	addEdge(graph2, a2, b2, 0);
	addEdge(graph2, a2, c2, 0);
	addEdge(graph2, a2, d2, 0);
	addEdge(graph2, a2, i2, 0);
	addEdge(graph2, a2, h2, 0);

	addEdge(graph2, b2, d2, 0);
	addEdge(graph2, b2, e2, 0);
	addEdge(graph2, b2, f2, 0);
	addEdge(graph2, b2, c2, 0);

	addEdge(graph2, c2, f2, 0);
	addEdge(graph2, c2, g2, 0);
	addEdge(graph2, c2, h2, 0);

	addEdge(graph2, i2, j2, 0);
	addEdge(graph2, i2, l2, 0);
	addEdge(graph2, i2, d2, 0);
	addEdge(graph2, i2, h2, 0);

	addEdge(graph2, d2, j2, 0);
	addEdge(graph2, d2, e2, 0);

	addEdge(graph2, e2, j2, 0);
	addEdge(graph2, e2, k2, 0);
	addEdge(graph2, e2, f2, 0);

	addEdge(graph2, f2, k2, 0);
	addEdge(graph2, f2, g2, 0);

	addEdge(graph2, g2, k2, 0);
	addEdge(graph2, g2, l2, 0);
	addEdge(graph2, g2, h2, 0);

	addEdge(graph2, h2, l2, 0);

	addEdge(graph2, l2, j2, 0);
	addEdge(graph2, l2, k2, 0);

	addEdge(graph2, k2, j2, 0);

	printf("Initial graph:\n");
	afficherGraphe(graph);
	// afficherGraphe(graph2);

	// STEP 1
	populateStacks(graph, s4, s5);
	// populateStacks(graph2, s4, s5);

	// STEP 2
	printf("----------In step2\n");
	printf("Inside s4 : ");
	for (int i = 0; i < s4->size; ++i)
	{
		printf("%d ",s4->vertices[i]->label );
	}
	printf("\n");
	printf("Inside s5 : ");
	for (int i = 0; i < s5->size; ++i)
	{
		printf("%d ",s5->vertices[i]->label );
	}
	printf("\n");
	printf("Inside sd : ");
	for (int i = 0; i < sd->size; ++i)
	{
		printf("%d ",sd->vertices[i]->label );
	}
	printf("\n");

	stack4ToStackD(graph, s4, sd, s5);
	// stack4ToStackD(graph2, s4, sd, s5);
	printf("----------Out step2\n");
	printf("Inside s4 : ");
	for (int i = 0; i < s4->size; ++i)
	{
		printf("%d ",s4->vertices[i]->label );
	}
	printf("\n");
	printf("Inside s5 : ");
	for (int i = 0; i < s5->size; ++i)
	{
		printf("%d ",s5->vertices[i]->label );
	}
	printf("\n");
	printf("Inside sd : ");
	for (int i = 0; i < sd->size; ++i)
	{
		printf("%d ",sd->vertices[i]->label );
	}
	printf("\n");

	printf("Graph after step2:\n");
	afficherGraphe(graph);
	printf("graph nbs : %d, nba : %d, maxS : %d\n", graph->nbS, graph->nbA, graph->maxS);
	// afficherGraphe(graph2);

	// STEP 3
	assert(isStackEmpty(s4)); // deg(graph) >= 5

	// The graph is empty
	if (graph->nbS == 0) {
		// STEP 5
		
		SOMMET *v;
		Color c[5] = {0, 1, 2, 3, 4}; // Create an array with all colors
		Color temp[5];
		while (!isStackEmpty(sd)) {
			printf("Inside sd : ");
			for (int i = 0; i < sd->size; ++i)
			{
				printf("%d ",sd->vertices[i]->label );
			}
			printf("\n");
			printf("Inside sTrash : ");
			for (int i = 0; i < sTrash->size; ++i)
			{
				printf("%d ",sTrash->vertices[i]->label );
			}
			printf("\n");
			v = popStack(sd);
			
			printf("neighbour of %d colors = [ ", v->label);
			for (ELTADJ *padj = v->adj; padj != NULL; padj = padj->suivant) {
				// printf("Dest : %d, info : %d\n",padj->dest, padj->info);
				printf("%d\n", padj->dest );
				// SOMMET *neighbour = findVertexWithLabelInsideStack(sTrash, padj->dest);
				// if (neighbour == NULL){
				// 	SOMMET *neighbour = findVertexWithLabelInsideStack(sd, padj->dest);
				// }
				// printf(" N : %d -> %d ", padj->dest, neighbour->info);
			}
			printf("]\n");
			pushStack(sTrash, v);
		}
	} else {
		mergeVertices(graph, s5, sd);
		// mergeVertices(graph2, s5, sd);
	}

	return 0;
}