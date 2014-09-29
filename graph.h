#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#define Vertex int
#define SIZE 50
#define ROUNDS 20
static int count;

// NODE is an element of the list
// LINK is a pointer to NODE structure
typedef struct node *Link;

// visited = -1 IF vertex was not visited yet
// visited >= 0 IF vertex was already visited
// ADJ is a pointer to list of NODEs
typedef struct {
  Link adj;
  int visited;
} VertexList;

// NODE is an element of this list
// W is the index of the vertex
// WEIGHT is the weight related to the path
// NEXT is a pointer to next element
struct node {
  Vertex w;
  double weight;
  Link next;
};

// DIGRAPH is the main structure
// V is the number of vertexes
// A is the number of arcs (or paths)
// LIST is a pointer to VERTEXLIST
struct digraph {
   int V;
   int A;
   VertexList *list;
};

typedef struct digraph *Digraph;
int paths[SIZE][SIZE + 2];

Link NewArc(Vertex nw, Link nnext, double weight){
  Link x = malloc (sizeof(*x));
  x->w = nw;
  x->weight = weight;
  x->next = nnext;
  return x;
}

Digraph initDigraph(int V){
  int v;
  Digraph G = (Digraph)malloc(sizeof(*G));
  G->V = V;
  G->A = 0;
  G->list = (VertexList *) malloc(V* sizeof(VertexList));

  for (v=0; v < V; v++){
    G->list[v].visited = -1;
    G->list[v].adj = NULL;
  }
  return G;
}

void insertArc(Digraph G, Vertex v, Vertex w, double weight){
  Link p;
  if (v == w) return;
  if ((v > G->V-1) || (w > G->V-1)) return;

  for (p = G->list[v].adj; p != NULL; p = p->next){
    if (p->w == w) return;
  }

  G->list[v].adj = NewArc(w, G->list[v].adj, weight);
  G->A++;
}

void printDigraph(Digraph G){
  Link p;
  Vertex i;

  printf("\nGraph vertices and adjacences:");
  for (i = 0; i<G->V; i++){
    printf("\nVertex %d (%d):\n", i, G->list[i].visited);
    for (p = G->list[i].adj; p != NULL; p = p->next){
      printf("   -> %d (weight %.1lf)\n", p->w, p->weight);
      if (p->next == NULL)
      printf("   (end)\n");
    }
  }
  printf("\n");
}

void insertEdge(Digraph G, Vertex v, Vertex w, double weight){
  insertArc(G, v, w, weight);
  insertArc(G, w, v, weight);
}

void initPaths(){
  int i, j;
  for(i = 0; i < SIZE; i++){
    for (j = 0; j < SIZE + 2; j++){
      paths[i][j] = -1;
    }
  }
}

void insertGene(int row, int column, int value){
  paths[row][column] = value;
}

void printPaths(int round){
  int i, j;
  printf("Round: %d\n", round);
  for(i = 0; i < SIZE; i++){
    for(j = 0; j < SIZE + 2; j++){
      printf("%d ", paths[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}
