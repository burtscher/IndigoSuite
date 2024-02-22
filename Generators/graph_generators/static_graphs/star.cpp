#include "graphGenerator.h"
#include "filter_nume.h"
/**************************************************************************
g1: n vertices, n-1 edges, and a random vertex r where each vertex has one
outgoing edge and the destination of the edge is always r (vertex r has
no outgoing edge)
***************************************************************************
g2: symmetric version of g1, the random vertex r has outgoing edges to
other vertices, the other vertices have no outgoing edges
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
  const int m = n - 1;
  if (filter_nume(m)) {
    const int seed = atoi(argv[3]);

    // randomly generate a destination for all (n - 1) edges
    std::set<int>* const edges1 = new std::set<int> [n];
    std::set<int>* const edges2 = new std::set<int> [n];
    std::set<int>* const edges3 = new std::set<int> [n];
    srand(seed);
    const int dst = rand() % n;
    for (int i = 0; i < n; i++) {
      const int src = i;
      if (i != dst) {
        edges1[src].insert(dst);
        edges2[dst].insert(src);
        edges3[src].insert(dst);
        edges3[dst].insert(src);
      }
    }

    if ((flag == 0) || (flag == 2)) {
      printf("\nStar\n");
      char name1[256];
      sprintf(name1, "star_%dn_%de.egr", n, m);
      saveAndPrint(n, m, name1, edges1);

      printf("\nCounter star\n");
      char name2[256];
      sprintf(name2, "counter_star_%dn_%de.egr", n, m);
      saveAndPrint(n, m, name2, edges2);
    }

    if ((flag == 0) || (flag == 1)) {
      printf("\nUndirected star\n");
      char name3[256];
      sprintf(name3, "undirect_star_%dn_%de.egr", n, m * 2);
      saveAndPrint(n, m * 2, name3, edges3);
    }
    delete [] edges1;
    delete [] edges2;
    delete [] edges3;
  }

  return 0;
}
