#if defined(GRAPHES_H)
#else
#define GRAPHES_H

#include <stdbool.h>
#include <assert.h>

#define MAX 10000

typedef enum Color_t {
   RED,
   GREEN,
   BLUE,
   YELLOW,
   PURPLE
}Color;

struct eltadj {
   int dest;
   Color info;
   struct eltadj *suivant;
};

struct sommet {
   int label;
   Color info;
   int degree; // inner and outer degree
   struct sommet *suivant;
   struct eltadj *adj;
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

int ajouterSommet(GRAPHE *, int info);

int ajouterArc(GRAPHE *, int a, int b, int info);

int supprimerSommet(GRAPHE *, int label);

int supprimerArc(GRAPHE *, int a, int b);

bool addEdge(GRAPHE *g, int a, int b, int info);

void supprimerGraphe(GRAPHE *);

bool removeEdge(GRAPHE *g, int a, int b);

void afficherGraphe(GRAPHE *);

int lireFichier(char *nomf, GRAPHE *);

const char *colorToString(Color color);

#endif