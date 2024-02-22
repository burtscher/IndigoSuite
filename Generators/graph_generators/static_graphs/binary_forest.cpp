#include "graphGenerator.h"
#include "filter_nume.h"
/**************************************************************************
g1: generate disconnected binary trees with n vertices, point from parent
to children
***************************************************************************
g2: reversed-edge version of g1
***************************************************************************
g3: undirected version of g1
**************************************************************************/

int main(int argc, char* argv[])
{
  // process the command line
  if (argc < 4) {fprintf(stderr, "USAGE: %s number_of_vertices directed random_seed\n", argv[0]); exit(-1);}
  const int n = atoi(argv[1]);
  if (n < 2) {fprintf(stderr, "ERROR: need at least 2 vertices\n"); exit(-1);}
  const int flag = atoi(argv[2]);
  int m = 0;
  const int seed = atoi(argv[3]);

  // create a random map to shuffle the vertex IDs
  int* const map = new int [n];
  for (int i = 0; i < n; i++) {
    map[i] = i;
  }
  std::mt19937 gen(seed);
  shuffle(map, map + n, gen);

  std::set<int>* const edges1 = new std::set<int> [n];
  std::set<int>* const edges2 = new std::set<int> [n];
  std::set<int>* const edges3 = new std::set<int> [n];
  srand(seed);
  const int p = 5;
  int count = 0;
  for (int i = 0; i < n && count < n; i++) {
    bool left = rand() % p;
    bool right = rand() % p;
    int src = i;
    int dst;

    if (left > 0) {
      dst = count + 1;
      if (dst < n) {
        edges1[map[src]].insert(map[dst]);
        m++;
        count = dst;
      }
    }
    if (right > 0) {
      dst = count + 1;
      if (dst < n) {
        edges1[map[src]].insert(map[dst]);
        m++;
        count = dst;
      }
    }
    if (i == count) {
      count++;
    }
  }
  if (filter_nume(m) && ((flag == 0) || (flag == 2))) {
    printf("\nBinary forest\n");
    char name1[256];
    sprintf(name1, "binary_forest_%dn_%de.egr", n, m);
    saveAndPrint(n, m, name1, edges1);

    printf("\nCounter binary forest\n");
    char name2[256];
    sprintf(name2, "counter_binary_forest_%dn_%de.egr", n, m);
    saveAndPrint(n, m, name2, edges2);
  }

  if (filter_nume(m * 2) && ((flag == 0) || (flag == 1))) {
    printf("\nUndirected binary forest\n");
    char name3[256];
    sprintf(name3, "undirect_binary_forest_%dn_%de.egr", n, m * 2);
    saveAndPrint(n, m * 2, name3, edges3);
  }

  delete [] edges2;
  delete [] edges1;
  delete [] edges3;

  return 0;
}