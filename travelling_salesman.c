#include "genetic.h"

int main(int argc, char *argv[])
{
  int i, j, k;

  Digraph G;

  G = initDigraph(SIZE);

  for(i = 0; i < SIZE; i++){
    for(j = i + 1; j < SIZE; j++){
      int random = 1 + (int) (1000.0 * (rand() / (RAND_MAX + 1.0)));
      insertEdge(G, i, j, random);
    }
  }

  initPaths();

  printf("Nearest neighbour!\n");
  for(i = 0; i < SIZE; i++){
    printf("Partindo de %d: \n", i);
    vizinho_mais_proximo(G,i);

    for (k = 0; k < G->V; k++){
      G->list[k].visited = -1;
    }
    printf("\n");
  }

  printf("Genetic algorithm with %d rounds!\n", ROUNDS);
  runGenetic(G, ROUNDS);

  printf("Best path found\n");
  printMenor();
  return 0;
}

