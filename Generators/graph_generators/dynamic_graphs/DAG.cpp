#include "graphGenerator.h"
/**************************************************************************
g1: directed acyclic graph with n vertices and m edges, each edge is from
higher priority to lower priority
***************************************************************************
g2: symmetric version of g1, reverse the source and destination of each
edge
**************************************************************************/

int main(int argc, char* argv[])
{
  // process the command line
  if (argc < 5) {fprintf(stderr, "USAGE: %s number_of_vertices number_of_edges directed random_seed \n", argv[0]); exit(-1);}
  const int n = atoi(argv[1]);
  if (n < 2) {fprintf(stderr, "ERROR: need at least 2 vertices\n"); exit(-1);}
  const int m = atoi(argv[2]);
  if ((m <= 0) || (m >= n * (n - 1) / 2)) {fprintf(stderr, "ERROR: number of edges out of range\n"); exit(-1);}
  const int flag = atoi(argv[3]);
  const int seed = atoi(argv[4]);

  // create a random map to shuffle the vertex IDs
  int* const map = new int [n];
  for (int i = 0; i < n; i++) {
    map[i] = i;
  }
  std::mt19937 gen(seed);
  shuffle(map, map + n, gen);

  // randomly generate edges
  std::set<int>* const edges1 = new std::set<int> [n];
  std::set<int>* const edges2 = new std::set<int> [n];
  for (int i = 0; i < m; i++) {
    int src, dst;
    do {
      do {
        src = rand() % n;
        dst = rand() % n;
      } while (src == dst);
      if (map[src] < map[dst]) {
        std::swap(src, dst);
      }
    } while (edges1[src].find(dst) != edges1[src].end());
    edges1[src].insert(dst);
    edges2[dst].insert(src);
  }

  if ((flag == 0) || (flag == 2)) {
    printf("\nDAG\n");
    char name1[256];
    sprintf(name1, "DAG_%dn_%de.egr", n, m);
    saveAndPrint(n, m, name1, edges1);

    printf("\nCounter DAG\n");
    char name2[256];
    sprintf(name2, "counterDAG_%dn_%de.egr", n, m);
    saveAndPrint(n, m, name2, edges2);
  }

  delete [] edges1;
  delete [] edges2;

  return 0;
}