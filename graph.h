#if defined(GRAPHES_H)
#else
#define GRAPHES_H

#include <stdbool.h>
#include <assert.h>
#include <stddef.h>

typedef enum Color_t {
   RED,
   GREEN,
   BLUE,
   YELLOW,
   PURPLE
} Color;

typedef struct {
   double x, y;
} Position;

struct Vertex {
   int label;
   Color color;
   int degree; // inner and outer degree
   Position position;
   struct Vertex *next;
   struct AdjList *adj; // the adjacency list (circular list)
   size_t adjSize; // the size of the adjacency list
   struct Vertex *mergedWith;
};

struct AdjList {
   struct Vertex *vertex;
   // The angle of the neighbour relative to this vertex
   // From -pi/2 to pi/2 for now
   double relative_angle;
   struct AdjList *next;
};

struct Graph {
   int nVertices;
   int nArcs;
   int maxVertices;
   struct Vertex *firstVertex;
   struct Vertex *lastVertex;
};

typedef struct Graph Graph;
typedef struct Vertex Vertex;
typedef struct AdjList AdjList;

void initializeGraph(Graph *g);

Vertex *addVertex(Graph *g, Position position);

int addArc(Graph *g, Vertex *a, Vertex *b);

int removeVertex(Graph *g, Vertex *a);

int removeArc(Vertex *a, Vertex *b);

bool addEdge(Graph *g, Vertex *a, Vertex *b);

void printGraph(Graph *g);

const char *colorToString(Color color);

#endif
