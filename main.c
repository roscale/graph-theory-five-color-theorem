#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "Stack.h"
#include "graph.h"
#include "tools.h"

static void colorGraph(Graph *g);

// https://en.wikipedia.org/wiki/Five_color_theorem
int main() {
	// ------------------------------------------ Graph example 1 ------------------------------------------
	Graph *graph1 = allocateMemory(sizeof(Graph));
	initializeGraph(graph1);

	// We don't need to assign vertex positions for this type of graph
	Position placeholder = {0, 0};

	{
		// We receive pointers instead of labels
		Vertex *a = addVertex(graph1, placeholder);
		Vertex *b = addVertex(graph1, placeholder);
		Vertex *c = addVertex(graph1, placeholder);
		Vertex *d = addVertex(graph1, placeholder);
		Vertex *e = addVertex(graph1, placeholder);
		Vertex *f = addVertex(graph1, placeholder);

		addEdge(graph1, a, b);
		addEdge(graph1, a, c);
		addEdge(graph1, b, c);

		addEdge(graph1, a, f);
		addEdge(graph1, a, e);
		addEdge(graph1, a, d);
		addEdge(graph1, c, d);
		addEdge(graph1, b, d);
		addEdge(graph1, d, f);
		addEdge(graph1, e, f);
	}

	//  ------------------------------------------ Graph exemple 2 ( 5-complet )  ------------------------------------------
	// https://i.stack.imgur.com/rO3SR.png
	// It is a planar graph with a minimum degree of 5
	Graph *graph2 = allocateMemory(sizeof(Graph));
	initializeGraph(graph2);

	{
		Position aPos = {-4, 0};
		Position bPos = {0, 6};
		Position cPos = {4, 0};
		Position dPos = {0, 1};
		Position ePos = {-1, 2};
		Position fPos = {-1, 3};
		Position gPos = {0, 4};
		Position hPos = {1, 3};
		Position iPos = {1, 2};
		Position jPos = {0, 2};
		Position kPos = {-0.5, 3};
		Position lPos = {0.5, 3};

		Vertex *a = addVertex(graph2, aPos);
		Vertex *b = addVertex(graph2, bPos);
		Vertex *c = addVertex(graph2, cPos);
		Vertex *d = addVertex(graph2, dPos);
		Vertex *e = addVertex(graph2, ePos);
		Vertex *f = addVertex(graph2, fPos);
		Vertex *g = addVertex(graph2, gPos);
		Vertex *h = addVertex(graph2, hPos);
		Vertex *i = addVertex(graph2, iPos);
		Vertex *j = addVertex(graph2, jPos);
		Vertex *k = addVertex(graph2, kPos);
		Vertex *l = addVertex(graph2, lPos);

		addEdge(graph2, a, b);
		addEdge(graph2, a, c);
		addEdge(graph2, a, d);
		addEdge(graph2, a, e);
		addEdge(graph2, a, f);

		addEdge(graph2, b, c);
		addEdge(graph2, b, f);
		addEdge(graph2, b, g);
		addEdge(graph2, b, h);

		addEdge(graph2, c, d);
		addEdge(graph2, c, i);
		addEdge(graph2, c, h);

		addEdge(graph2, i, j);
		addEdge(graph2, i, l);
		addEdge(graph2, i, d);
		addEdge(graph2, i, h);

		addEdge(graph2, d, j);
		addEdge(graph2, d, e);

		addEdge(graph2, e, k);
		addEdge(graph2, e, f);
		addEdge(graph2, e, j);

		addEdge(graph2, f, k);
		addEdge(graph2, f, g);

		addEdge(graph2, g, k);
		addEdge(graph2, g, l);
		addEdge(graph2, g, h);

		addEdge(graph2, h, l);

		addEdge(graph2, l, j);
		addEdge(graph2, l, k);

		addEdge(graph2, k, j);
	}


	//  ------------------------------------------ Graph exemple 3 ( 3-complet )  ------------------------------------------
	//https://www.cs.sfu.ca/~ggbaker/zju/math/planar.html (Q3)
	Graph *graph3 = allocateMemory(sizeof(Graph));
	initializeGraph(graph3);

	{
		Vertex *a = addVertex(graph3, placeholder);
		Vertex *b = addVertex(graph3, placeholder);
		Vertex *c = addVertex(graph3, placeholder);
		Vertex *d = addVertex(graph3, placeholder);
		Vertex *e = addVertex(graph3, placeholder);
		Vertex *f = addVertex(graph3, placeholder);
		Vertex *g = addVertex(graph3, placeholder);
		Vertex *h = addVertex(graph3, placeholder);

		addEdge(graph3, a, b);
		addEdge(graph3, a, d);
		addEdge(graph3, a, e);

		addEdge(graph3, d, h);

		addEdge(graph3, c, b);
		addEdge(graph3, c, d);
		addEdge(graph3, c, g);

		addEdge(graph3, b, f);

		addEdge(graph3, e, f);
		addEdge(graph3, e, h);

		addEdge(graph3, g, f);
		addEdge(graph3, g, h);
	}

	colorGraph(graph2);

	return 0;
}

static void colorGraph(Graph *g) {
	Stack *s4 = createStack(100);
	Stack *s5 = createStack(100);
	Stack *sd = createStack(100);
	Stack *sTrash = createStack(100);

	printf("Initial graph:\n");
	printGraph(g);

	// STEP 1
	populateStacks(g, s4, s5);

	// While the graph is not empty
	while (true) {
		// STEP 2
		stack4ToStackD(g, s4, sd, s5);

		// If the graph is empty
		if (g->nVertices == 0) {
			break;
		}

		// STEP 3
		assert(isStackEmpty(s4)); // deg(graph) >= 5
		mergeVertices(g, s4, s5, sd);
	}

	// We finished parsing the graph
	// Assign colors to the vertices
	Vertex *v;
	while (!isStackEmpty(sd)) {
		v = popStack(sd);

		// Color merged vertex
		if (v->mergedWith != NULL) {
			v->color = v->mergedWith->color;
			pushStack(sTrash, v);
			continue;
		}

		// Choose a color different than all its neighbours
		Color c[5] = {0, 1, 2, 3, 4};
		AdjList *padj = v->adj;
		for (size_t i = 0; i < v->adjSize; i++, padj = padj->next) {
			if (padj->vertex->color >= 0 && padj->vertex->color < 5) {
				c[padj->vertex->color] = -1;
			}
		}

		v->color = getColorFromArray(c);
		pushStack(sTrash, v);
	}

	// Print the final colors
	printf("\n----- Affichage des couleurs des différents sommets -----\n");
	while (!isStackEmpty(sTrash)) {
		v = popStack(sTrash);
		printf("v(%d):\t%s\n", v->label, colorToString(v->color));
	}
	printf("\n");

	// Free all the allocated memory
	freeMemory();
}
