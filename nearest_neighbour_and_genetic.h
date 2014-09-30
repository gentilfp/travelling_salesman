#include "graph.h"

int has_it(int son[], int value, int max){
  int i;
  for (i = 0; i <= max; i++){
    if (son[i] == value){
      return 1;
    }
  }
  return 0;
}

void calculateSon(int son1[], int father1[], int father2[]){
  int i,j ;
  // son1 = father1 x father2, son2 = father1 x father3, son3 = father2 x father3
  for (i = 0; i < SIZE + 2; i++){
    if (i == SIZE){
      son1[i] = son1[0];
    } else if (i % 2 == 0){
      if (has_it(son1, father1[i], i) == 0){
        // even = pega do father1
        son1[i] = father1[i];
      } else {
        if (has_it(son1, father2[i], i) == 0){
          // insert from father1
          son1[i] = father2[i];
        } else {
          // back until finds one available
          for (j = i; j >= 0; j--){
            int city1 = father1[j];
            int city2 = father2[j];

            if (has_it(son1, city1, i) == 0){
              son1[i] = city1;
              break;
            } else if (has_it(son1, city2, i) == 0) {
              son1[i] = city2;
              break;
            }
          }
        }
      }
    } else {
      if (has_it(son1, father2[i], i) == 0){
        // can insert
        son1[i] = father2[i];
      } else {
        if (has_it(son1, father1[i], i) == 0){
          // insert from father1
          son1[i] = father1[i];
        } else {
          // back until finds one available
          for (j = i; j >= 0; j--){
            int city1 = father1[j];
            int city2 = father2[j];

            if (has_it(son1, city1, i) == 0){
              son1[i] = city1;
              break;
            } else if (has_it(son1, city2, i) == 0){
              son1[i] = city2;
              break;
            }
          }
        }
      }
    }
  }
}

void calculateSonWeight(int son[], Digraph G){
  int path = 0;
  int j;

  Link p;
  Vertex i;

  for (i = 0; i < SIZE + 1; i++){
    for (p = G->list[son[i]].adj; p != NULL; p = p->next){
      if (p->w == son[i+1]){
        path += p->weight;
        break;
      }
    }
  }
  son[SIZE + 1] = path;
}

void mutation(int son[]){
  int newSize = SIZE * 0.1;
  int i;

  if (newSize == 0){
    newSize = 2;
  } else if (newSize % 2 != 0){
    newSize++;
  }

  int vetGenesMutation[newSize];
  for (i = 0; i < newSize; i++){
    int random = 1 + (int) (SIZE * (rand() / (RAND_MAX + 1.0)));
    if (random == SIZE){
      random--;
    }
    vetGenesMutation[i] = random;
  }

  // mutates
  for (i = 0; i < newSize - 1; i++){
    int gene1 = vetGenesMutation[i];
    int gene2 = vetGenesMutation[i + 1];

    int aux;
    aux = son[gene2];
    son[gene2] = son[gene1];
    son[gene1] = aux;
  }
}

void printMenor(){
  int menor = INT_MAX;
  int index = 0;
  int i;

  // finds lowest
  for (i = 0; i < SIZE; i++){
    if (menor > paths[i][SIZE + 1]){
      menor = paths[i][SIZE + 1];
      index = i;
    }
  }

  // print
  for (i = 0; i < SIZE + 1; i++){
    printf("%d -> ", paths[index][i]);
  }
  printf("%d\n", paths[index][SIZE + 1]);
}

void runGenetic(Digraph G, int rounds)
{
  // =>> MORTALITY = 30%
  // =>> MUTATION = 10%

  int m;
  for (m = 0; m < rounds; m++){
    printPaths(m);

    int i, j;
    int column = SIZE + 1;

    int min1, min2, min3;
    int max1, max2, max3;
    int indexMin1, indexMin2, indexMin3, indexMax1, indexMax2, indexMax3;

    int son1[SIZE + 2];
    int son2[SIZE + 2];
    int son3[SIZE + 2];

    int father1[SIZE + 2];
    int father2[SIZE + 2];
    int father3[SIZE + 2];

    for (i = 0; i < SIZE + 2; i++){
      father1[i] = -1;
      father2[i] = -1;
      father3[i] = -1;
    }

    max1 = INT_MIN;
    max2 = INT_MIN;
    max3 = INT_MIN;
    min1 = INT_MAX;
    min2 = INT_MAX;
    min3 = INT_MAX;

    // find max3 and min3
    for (i = 0; i < SIZE; i++){
      // max
      if (paths[i][column] > max1){
        max3 = max2;
        max2 = max1;
        max1 = paths[i][column];
        indexMax1 = i;
      } else if (paths[i][column] > max2){
        max3 = max2;
        max2 = paths[i][column];
        indexMax2 = i;
      } else if (paths[i][column] > max3){
        max3 = paths[i][column];
        indexMax3 = i;
      }

      // min
      if (paths[i][column] < min1){
        min3 = min2;
        min2 = min1;
        min1 = paths[i][column];
        indexMin1 = i;
      } else if (paths[i][column] < min2){
        min3 = min2;
        min2 = paths[i][column];
        indexMin2 = i;
      } else if (paths[i][column] < min3){
        min3 = paths[i][column];
        indexMin3 = i;
      }
    }

    // set fathers
    for (i = 0; i < SIZE + 2; i++){
      father1[i] = paths[indexMin1][i];
      father2[i] = paths[indexMin2][i];
      father3[i] = paths[indexMin3][i];
    }

    calculateSon(son1, father1, father2);
    calculateSon(son2, father1, father3);
    calculateSon(son3, father2, father3);
    calculateSonWeight(son1, G);
    calculateSonWeight(son2, G);
    calculateSonWeight(son3, G);

    // mutation
    mutation(son1);
    mutation(son2);
    mutation(son3);
    calculateSonWeight(son1, G);
    calculateSonWeight(son2, G);
    calculateSonWeight(son3, G);

    // seta fathers
    for (i = 0; i < SIZE + 2; i++){
      paths[indexMax3][i] = son3[i];
      paths[indexMax2][i] = son2[i];
      paths[indexMax1][i] = son1[i];
    }
  }
}

void nearest_neighbour(Digraph G, Vertex v)
{
  Link p;
  int path = 0;
  int option = -1;

  Vertex init = v;
  Vertex next;
  double lowestWeight = -1;

  int column = 0;
  int row = v;

  G->list[v].visited = 1;
  printf("%d -> ", v);
  insertGene(v, count, v);

  do{
    column++;

    for (p = G->list[v].adj; p != NULL; p = p->next){
      if (G->list[p->w].visited == -1){
        if (lowestWeight == -1 && p->w != init){
          lowestWeight = p->weight;
          next = p->w;
          option = 1;
        } else if (p->weight < lowestWeight && p->w != init){
          lowestWeight = p->weight;
          next = p->w;
          option = 1;
        }
      }
    }

    if(option == 1){
      G->list[next].visited = 1;
      path = path + lowestWeight;
      v = next;
      printf("%d -> ", v);
      insertGene(row, column, v);
    } else if(option == -1){
      G->list[init].visited = 1;
      p = G->list[v].adj;

      while(p->w != init)
          p = p->next;

      path = path + p->weight;
      v = init;
      printf("%d! - ", v);
      insertGene(row, column, v);
    }

    lowestWeight = -1;
    option = -1;
  }
  while(v != init);

  printf("Path value: %d", path);
  insertGene(row, SIZE + 1, path);
}
