#include "graphGenerator.h"
#include "filter_nume.h"
/**************************************************************************
g1: directed graph with n vertices where the max in and out degree
is capped at k
***************************************************************************
g2: reversed-edge version of g1
***************************************************************************
g3: undirected version of g1
**************************************************************************/

int main(int argc, char* argv[])
{
  // process the command line
  if (argc < 6) {fprintf(stderr, "USAGE: %s number_of_vertices number_of_edges directed random_seed max_degree\n", argv[0]); exit(-1);}
  const int n = atoi(argv[1]);
  if (n < 2) {fprintf(stderr, "ERROR: need at least 2 vertices\n"); exit(-1);}
  const int flag = atoi(argv[3]);
  const int seed = atoi(argv[4]);
  const int maxD = atoi(argv[5]);
  if (maxD < 1) {fprintf(stderr, "ERROR: maximum degree must be at least 1\n"); exit(-1);}
  int m = 0;

  srand(seed);
  std::set<int>* const edges1 = new std::set<int> [n];
  std::set<int>* const edges2 = new std::set<int> [n];
  std::set<int>* const edges3 = new std::set<int> [n];

  // generate edges
  for (int i = 0; i < n; i++) {
    int src, dst;
    src = i;
    for (int j = 0; j < maxD / 2; j++) {
      dst = rand() % n;
      if (src != dst && edges3[dst].size() < maxD && edges3[src].size() < maxD) {
        edges1[src].insert(dst);
        edges2[dst].insert(src);
        edges3[src].insert(dst);
        edges3[dst].insert(src);
      }
    }
    m += edges1[src].size();
  }

  if (filter_nume(m) && ((flag == 0) || (flag == 2))) {
    printf("\nOne outing edge\n");
    char name1[256];
    sprintf(name1, "%dmax_out_degree_%dn_%de.egr", maxD, n, m);
    saveAndPrint(n, m, name1, edges1);


    printf("\nOne incoming edge\n");
    char name2[256];
    sprintf(name2, "%dmax_in_degree_%dn_%de.egr", maxD, n, m);
    saveAndPrint(n, m, name2, edges2);
  }

  if (filter_nume(m * 2) && ((flag == 0) || (flag == 1))) {
    printf("\nUndirected in and out\n");
    char name3[256];
    sprintf(name3, "undirect_%dmax_degree_%dn_%de.egr", maxD, n, m * 2);
    saveAndPrint(n, m * 2, name3, edges3);
  }


  delete [] edges1;
  delete [] edges2;
  delete [] edges3;

  return 0;
}