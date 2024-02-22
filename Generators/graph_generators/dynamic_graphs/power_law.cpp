#include "graphGenerator.h"
#include "filter_nume.h"
/**************************************************************************
g1: n vertices and m edges (with a uniform distribution)
***************************************************************************
g2: symmetric version of g1, reverse the source and destination of each
edge
***************************************************************************
g3: undirected version of g1
**************************************************************************/

int main(int argc, char* argv[])
{
  // process the command line
  if (argc < 5) {fprintf(stderr, "USAGE: %s number_of_vertices number_of_edges directed random_seed\n", argv[0]); exit(-1);}
  const int n = atoi(argv[1]);
  if (n < 2) {fprintf(stderr, "ERROR: need at least 2 vertices\n"); exit(-1);}
  const int m = atoi(argv[2]);
  const int flag = atoi(argv[3]);
  if ((m <= 0) || (m > n * n / 3)) {fprintf(stderr, "ERROR: number of edges out of range\n"); exit(-1);}
  const int seed = atoi(argv[4]);

  // create a random map to shuffle the vertex IDs
  int* const map = new int [n];
  for (int i = 0; i < n; i++) {
    map[i] = i;
  }
  std::mt19937 gen(seed);
  shuffle(map, map + n, gen);
  double average_d = (m * 2.0) / n;

  std::default_random_engine generator;
  std::geometric_distribution<int> distribution(average_d / n);
  std::set<int>* const edges1 = new std::set<int> [n];
  std::set<int>* const edges2 = new std::set<int> [n];
  std::set<int>* const edges3 = new std::set<int> [n];
  for (int i = 0; i < m; i++) {
    int src, dst;
    do {
      do {
        src = distribution(generator);
      } while (src >= n);
      do {
        dst = distribution(generator);
      } while ((dst >= n) || (src == dst));
      src = map[src];
      dst = map[dst];
    } while (edges3[src].find(dst) != edges3[src].end());
    edges1[src].insert(dst);
    edges2[dst].insert(src);
    edges3[src].insert(dst);
    edges3[dst].insert(src);
  }

  if ((flag == 0) || (flag == 2)) {
    printf("\nRandom power-law graph\n");
    char name1[256];
    sprintf(name1, "power_law_%dn_%de.egr", n, m);
    saveAndPrint(n, m, name1, edges1);

    printf("\nCounter random power-law graph\n");
    char name2[256];
    sprintf(name2, "counter_power_law_%dn_%de.egr", n, m);
    saveAndPrint(n, m, name2, edges2);
  }

  if (filter_nume(m * 2) && ((flag == 0) || (flag == 1))) {
    printf("\nUndirected random power-law graph\n");
    char name3[256];
    sprintf(name3, "undirect_power_law_%dn_%de.egr", n, m * 2);
    saveAndPrint(n, m * 2, name3, edges3);
  }

  delete [] edges1;
  delete [] edges2;
  delete [] edges3;

  return 0;
}