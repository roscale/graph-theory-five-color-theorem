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

	// ------------------------------------------ Graph example 1 ------------------------------------------
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

	//  ------------------------------------------ Graph exemple 2 ( 5-complet )  ------------------------------------------
	// https://i.stack.imgur.com/rO3SR.png
	// It is a planar graph with a minimum degree of 5
	GRAPHE *graph2 = malloc(sizeof(GRAPHE));
	initialiserGraphe(graph2);

	Position a2Pos = {-4, 0};
	Position b2Pos = {0, 6};
	Position c2Pos = {4, 0};
	Position d2Pos = {0, 1};
	Position e2Pos = {-1, 2};
	Position f2Pos = {-1, 3};
	Position g2Pos = {0, 4};
	Position h2Pos = {1, 3};
	Position i2Pos = {1, 2};
	Position j2Pos = {0, 2};
	Position k2Pos = {-0.5, 3};
	Position l2Pos = {0.5, 3};

	SOMMET *a2 = ajouterSommet(graph2, a2Pos);
	SOMMET *b2 = ajouterSommet(graph2, b2Pos);
	SOMMET *c2 = ajouterSommet(graph2, c2Pos);
	SOMMET *d2 = ajouterSommet(graph2, d2Pos);
	SOMMET *e2 = ajouterSommet(graph2, e2Pos);
	SOMMET *f2 = ajouterSommet(graph2, f2Pos);
	SOMMET *g2 = ajouterSommet(graph2, g2Pos);
	SOMMET *h2 = ajouterSommet(graph2, h2Pos);
	SOMMET *i2 = ajouterSommet(graph2, i2Pos);
	SOMMET *j2 = ajouterSommet(graph2, j2Pos);
	SOMMET *k2 = ajouterSommet(graph2, k2Pos);
	SOMMET *l2 = ajouterSommet(graph2, l2Pos);

	// Returns 0 on success
	addEdge(graph2, a2, b2);
	addEdge(graph2, a2, c2);
	addEdge(graph2, a2, d2);
	addEdge(graph2, a2, e2);
	addEdge(graph2, a2, f2);

	addEdge(graph2, b2, c2);
	addEdge(graph2, b2, f2);
	addEdge(graph2, b2, g2);
	addEdge(graph2, b2, h2);

	addEdge(graph2, c2, d2);
	addEdge(graph2, c2, i2);
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

	//  ------------------------------------------ Graph exemple 3 ( 3-complet )  ------------------------------------------
	//https://www.cs.sfu.ca/~ggbaker/zju/math/planar.html (Q3)
	GRAPHE *graph3 = malloc(sizeof(GRAPHE));
	initialiserGraphe(graph3);

	SOMMET *a3 = ajouterSommet(graph3, placeholder);
	SOMMET *b3 = ajouterSommet(graph3, placeholder);
	SOMMET *c3 = ajouterSommet(graph3, placeholder);
	SOMMET *d3 = ajouterSommet(graph3, placeholder);
	SOMMET *e3 = ajouterSommet(graph3, placeholder);
	SOMMET *f3 = ajouterSommet(graph3, placeholder);
	SOMMET *g3 = ajouterSommet(graph3, placeholder);
	SOMMET *h3 = ajouterSommet(graph3, placeholder);

	// Returns 0 on success
	addEdge(graph3, a3, b3);
	addEdge(graph3, a3, d3);
	addEdge(graph3, a3, e3);

	addEdge(graph3, d3, h3);
	
	addEdge(graph3, c3, b3);
	addEdge(graph3, c3, d3);
	addEdge(graph3, c3, g3);

	addEdge(graph3, b3, f3);

	addEdge(graph3, e3, f3);
	addEdge(graph3, e3, h3);

	addEdge(graph3, g3, f3);
	addEdge(graph3, g3, h3);

	printf("Initial graph:\n");
	// afficherGraphe(graph);
	 afficherGraphe(graph2);
//	afficherGraphe(graph3);

	// STEP 1
	// populateStacks(graph, s4, s5);
	 populateStacks(graph2, s4, s5);
//	populateStacks(graph3, s4, s5);

	// STEP 2

	step2:

	// stack4ToStackD(graph, s4, sd, s5);
	 stack4ToStackD(graph2, s4, sd, s5);
//	stack4ToStackD(graph3, s4, sd, s5);

	// STEP 3
	assert(isStackEmpty(s4)); // deg(graph) >= 5

	// The graph is empty
	if (graph2->nbS == 0) {
		// STEP 5
		
		SOMMET *v;
		while (!isStackEmpty(sd)) {
			v = popStack(sd);

			// Color merged vertex
			if (v->mergedWith != NULL) {
				v->info = v->mergedWith->info;
				pushStack(sTrash, v);
				continue;
			}

			Color c[5] = {0, 1, 2, 3, 4};
			ELTADJ *padj = v->adj;
			for (size_t i = 0; padj != NULL && i < v->adjSize; i++, padj = padj->suivant) {
				// printf("Dest : %d, info : %d\n",padj->dest, padj->info);
				// printf("%d ", padj->vertex->info );
				if(padj->vertex->info >= 0 && padj->vertex->info < 5){
					c[padj->vertex->info] = -1;
				}
			}

			v->info = getColorFromArray(c);
			pushStack(sTrash, v);
		}

		printf("----------- Affichage des couleurs des diff somment : \n");
		while (!isStackEmpty(sTrash)){
			SOMMET *v = popStack(sTrash);
			printf("v(%d):%s ", v->label, colorToString(v->info) );
		}
		printf("\n");
	} else {
		// mergeVertices(graph, s5, sd);
		 mergeVertices(graph2, s5, sd);
		 goto step2;
//		mergeVertices(graph3, s5, sd);
	}

	return 0;
}