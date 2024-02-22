#include <stdio.h>
#include <random>
#include <set>
#include <algorithm>
#include "ECLgraph.h"

static ECLgraph toCSR(const int n, const int m, std::set<int>* edges)
{
  // convert to CSR graph format
  ECLgraph g;
  g.nodes = n;
  g.edges = m;
  g.nindex = new int [g.nodes + 1];
  g.nlist = new int [g.edges];
  g.eweight = NULL;
  int ecnt = 0;
  g.nindex[0] = 0;
  for (int i = 0; i < g.nodes; i++) {
    for (int dst: edges[i]) {
      g.nlist[ecnt] = dst;
      ecnt++;
    }
    g.nindex[i + 1] = ecnt;
  }
  return g;
}

static void printHistogram(const ECLgraph g)
{
  printf("number of vertices: %d\n", g.nodes);
  printf("number of edges: %d\n", g.edges);

  // count and sort the in-degree and out-degree
  int* const in_deg = new int [g.nodes];
  int* const out_deg = new int [g.nodes];
  for (int i = 0; i < g.nodes; i++) {
    in_deg[i] = 0;
  }
  for (int i = 0; i < g.nodes; i++) {
    out_deg[i] = g.nindex[i + 1] - g.nindex[i];
    const int beg = g.nindex[i];
    const int end = g.nindex[i + 1];
    for (int j = beg; j < end; j++) {
      const int v = g.nlist[j];
      in_deg[v]++;
    }
  }
  std::sort(in_deg, in_deg + g.nodes);
  std::sort(out_deg, out_deg + g.nodes);

  int d = in_deg[0];
  int count = 0;
  printf("=====in-degree histogram=====\n");
  printf("in-degree          frequency\n");
  for (int i = 0; i < g.nodes; i++) {
    if (d < in_deg[i]) {
      printf("%d%27d\n", d, count);
      d = in_deg[i];
      count = 1;
    } else if (d == in_deg[i]) {
      count++;
    }
  }
  printf("%d%27d\n", d, count);

  d = out_deg[0];
  count = 0;
  printf("=====out-degree histogram=====\n");
  printf("out-degree          frequency\n");
  for (int i = 0; i < g.nodes; i++) {
    if (d < out_deg[i]) {
      printf("%d%27d\n", d, count);
      d = out_deg[i];
      count = 1;
    } else if (d == out_deg[i]) {
      count++;
    }
  }
  printf("%d%27d\n", d, count);

  delete [] in_deg;
  delete [] out_deg;
  return;
}

static void saveAndPrint(const int n, const int m, const char* const fname, std::set<int>* edges)
{
  ECLgraph g = toCSR(n, m, edges);
  writeECLgraph(g, fname);
  printHistogram(g);
  freeECLgraph(g);
}