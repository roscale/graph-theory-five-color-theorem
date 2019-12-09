#if defined(GRAPHES_H)
#else
#define GRAPHES_H

#include <stdbool.h>

#define MAX 10000

typedef struct {
   double x, y;
} Position;

struct sommet {
   int label;
   int degree; // inner and outer degree
   Position position;
   struct sommet *suivant;
   struct eltadj *adj;
};

struct eltadj {
   struct sommet *vertex;
   // The angle of the neighbour relative to this vertex
   // From -pi/2 to pi/2 for now
   double relative_angle;
   struct eltadj *suivant;
};

struct graphe {
   int nbS;
   int nbA;
   int maxS;
   struct sommet *premierSommet;
   struct sommet *dernierSommet;
};

typedef struct graphe GRAPHE;
typedef struct sommet SOMMET;
typedef struct eltadj ELTADJ;

void initialiserGraphe(GRAPHE *);

SOMMET * ajouterSommet(GRAPHE *g, Position position);

int ajouterArc(GRAPHE *g, SOMMET *a, SOMMET *b);

int supprimerSommet(GRAPHE *g, SOMMET *a);

int supprimerArc(GRAPHE *g, SOMMET *a, SOMMET *b);

bool addEdge(GRAPHE *g, SOMMET *a, SOMMET *b);

void supprimerGraphe(GRAPHE *);

bool removeEdge(GRAPHE *g, SOMMET *a, SOMMET *b);

void afficherGraphe(GRAPHE *);

//int lireFichier(char *nomf, GRAPHE *);

#endif