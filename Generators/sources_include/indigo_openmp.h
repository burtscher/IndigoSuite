#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

/****************************************************************************************/

struct ECLgraph {
  int nodes;
  int edges;
  int* nindex;
  int* nlist;
};

struct ECLgraph readECLgraph(const char* const fname)
{
  struct ECLgraph g;
  int cnt;

  FILE* f = fopen(fname, "rb");  if (f == NULL) {fprintf(stderr, "ERROR: could not open file %s\n\n", fname);  exit(-1);}
  cnt = fread(&g.nodes, sizeof(g.nodes), 1, f);  if (cnt != 1) {fprintf(stderr, "ERROR: failed to read nodes\n\n");  exit(-1);}
  cnt = fread(&g.edges, sizeof(g.edges), 1, f);  if (cnt != 1) {fprintf(stderr, "ERROR: failed to read edges\n\n");  exit(-1);}
  printf("input graph: %d nodes and %d edges\n", g.nodes, g.edges);
  if ((g.nodes < 1) || (g.edges < 0)) {fprintf(stderr, "ERROR: node or edge count too low\n\n");  exit(-1);}

  g.nindex = (int*)malloc((g.nodes + 1) * sizeof(g.nindex[0]));
  g.nlist = (int*)malloc(g.edges * sizeof(g.nlist[0]));

  cnt = fread(g.nindex, sizeof(g.nindex[0]), g.nodes + 1, f);  if (cnt != g.nodes + 1) {fprintf(stderr, "ERROR: failed to read neighbor index list\n\n");  exit(-1);}
  cnt = fread(g.nlist, sizeof(g.nlist[0]), g.edges, f);  if (cnt != g.edges) {fprintf(stderr, "ERROR: failed to read neighbor list\n\n");  exit(-1);}
  fclose(f);
  return g;
}

void freeECLgraph(struct ECLgraph* g)
{
  if (g->nindex != NULL) free(g->nindex);
  if (g->nlist != NULL) free(g->nlist);
  //if (g->eweight != NULL) free(g->eweight);
  g->nindex = NULL;
  g->nlist = NULL;
}

/****************************************************************************************/

void omp_code(int* nindex, int* nlist, data_t* data1, data_t* data2, int n);
void serial_code(int* nindex, int* nlist, data_t* data1, data_t* data2, int n);
int verify_result(int* nindex, int* nlist, data_t* h_data1, data_t* h_data2, data_t* d_data1, data_t* d_data2, int numv, int nume);

/****************************************************************************************/

int main (int argc, char* argv[])
{
  // process command line
  if (argc != 3) {fprintf(stderr, "USAGE: %s input_file_name number_of_threads\n", argv[0]); exit(-1);}
  struct ECLgraph g = readECLgraph(argv[1]);
  int numt = atoi(argv[2]);

  // allocate and init two data arrays
  int n = g.nodes;
  int e = g.edges;
  int s = ((n) > (e) ? n : e);

  data_t* data1 = (data_t*)malloc(s * sizeof(data_t));
  data_t* data2 = (data_t*)malloc(s * sizeof(data_t));
  data_t* h_data1 = (data_t*)malloc(s * sizeof(data_t));
  data_t* h_data2 = (data_t*)malloc(s * sizeof(data_t));

  data1[0] = 0;
  h_data1[0] = 0;
  data2[0] = 0;
  h_data2[0] = 0;

  for (int i = 1; i < s; i++) {
    data1[i] = rand() % n;
    h_data1[i] = data1[i];
  }
  for (int i = 1; i < s; i++) {
    data2[i] = rand() % e;
    h_data2[i] = data2[i];
  }

  omp_set_num_threads(numt);
  // run tests
  omp_code(g.nindex, g.nlist, data1, data2, n);
  serial_code(g.nindex, g.nlist, h_data1, h_data2, n);

  // check result
  int ret = verify_result(g.nindex, g.nlist, h_data1, h_data2, data1, data2, n, e);
  if (ret == 1) {
    printf("result matches serial code\n");
  } else if (ret == 0) {
    printf("result differs from serial code\n");
  }

  // cleanup
  free(data1);
  free(data2);
  free(h_data1);
  free(h_data2);
  freeECLgraph(&g);
  return 0;
}
