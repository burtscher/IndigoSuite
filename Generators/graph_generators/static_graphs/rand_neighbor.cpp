#include "graphGenerator.h"
#include "filter_nume.h"
/**************************************************************************
g1: n vertices and n edges where each vertex has one outgoing edge but the
destination of the edge is random (with a uniform distribution)
***************************************************************************
g2: symmetric version of g1, n vertices and n edges where each vertex has
one incoming edge but the source of the edge is random (with a
uniform distribution)
***************************************************************************
g3: undirected version of g1
**************************************************************************/

int main(int argc, char* argv[])
{
  // process the command line
  if (argc < 4) {fprintf(stderr, "USAGE: %s number_of_vertices directed random_seed\n", argv[0]); exit(-1);}
  const int n = atoi(argv[1]);
  if (n < 2) {fprintf(stderr, "ERROR: need at least 2 vertices\n"); exit(-1);}
  const int m = n;
  const int flag = atoi(argv[2]);
  const int seed = atoi(argv[3]);

  // generate random edges with uniform distribution of destination
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, n - 1);
  std::set<int>* const edges1 = new std::set<int> [n];
  std::set<int>* const edges2 = new std::set<int> [n];
  std::set<int>* const edges3 = new std::set<int> [n];
  for (int i = 0; i < n; i++) {
    int src, dst;
    src = i;
    do {
      dst = distribution(generator);
    } while (src == dst);
    edges1[src].insert(dst);
    edges2[dst].insert(src);
    edges3[src].insert(dst);
    edges3[dst].insert(src);
  }

  if ((flag == 0) || (flag == 2)) {
    printf("\nRandom neighbors\n");
    char name1[256];
    sprintf(name1, "rand_neighbors_%dn_%de.egr", n, m);
    saveAndPrint(n, m, name1, edges1);

    printf("\nCounter random neighbors edge\n");
    char name2[256];
    sprintf(name2, "counter_rand_neighbors_%dn_%de.egr", n, m);
    saveAndPrint(n, m, name2, edges2);
  }

  if (filter_nume(m * 2) && ((flag == 0) || (flag == 1))) {
    printf("\nUndirected in and out\n");
    char name3[256];
    sprintf(name3, "undirect_rand_neighbors_%dn_%de.egr", n, m * 2);
    saveAndPrint(n, m * 2, name3, edges3);
  }
  delete [] edges2;
  delete [] edges1;
  delete [] edges3;

  return 0;
}