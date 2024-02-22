#include <cuda.h>
#include <cstdlib>
#include <cstdio>

/****************************************************************************************/

struct ECLgraph {
  int nodes;
  int edges;
  int* nindex;
  int* nlist;
};

ECLgraph readECLgraph(const char* const fname)
{
  ECLgraph g;
  int cnt;

  FILE* f = fopen(fname, "rb");  if (f == NULL) {fprintf(stderr, "ERROR: could not open file %s\n\n", fname);  exit(-1);}
  cnt = fread(&g.nodes, sizeof(g.nodes), 1, f);  if (cnt != 1) {fprintf(stderr, "ERROR: failed to read nodes\n\n");  exit(-1);}
  cnt = fread(&g.edges, sizeof(g.edges), 1, f);  if (cnt != 1) {fprintf(stderr, "ERROR: failed to read edges\n\n");  exit(-1);}
  printf("input graph: %d nodes and %d edges\n", g.nodes, g.edges);
  if ((g.nodes < 1) || (g.edges < 0)) {fprintf(stderr, "ERROR: node or edge count too low\n\n");  exit(-1);}

  g.nindex = (int*)malloc((g.nodes + 1) * sizeof(g.nindex[0]));
  g.nlist = (int*)malloc(g.edges * sizeof(g.nlist[0]));

  if ((g.nindex == NULL) || (g.nlist == NULL)) {fprintf(stderr, "ERROR: memory allocation failed\n\n");  exit(-1);}

  cnt = fread(g.nindex, sizeof(g.nindex[0]), g.nodes + 1, f);  if (cnt != g.nodes + 1) {fprintf(stderr, "ERROR: failed to read neighbor index list\n\n");  exit(-1);}
  cnt = fread(g.nlist, sizeof(g.nlist[0]), g.edges, f);  if (cnt != g.edges) {fprintf(stderr, "ERROR: failed to read neighbor list\n\n");  exit(-1);}
  fclose(f);
  return g;
}

void freeECLgraph(struct ECLgraph* g)
{
  if (g->nindex != NULL) free(g->nindex);
  if (g->nlist != NULL) free(g->nlist);
  g->nindex = NULL;
  g->nlist = NULL;
}

/****************************************************************************************/

__global__ void test_kernel(int* nindex, int* nlist, data_t* data1, data_t* data2, int n);
void serial_code(int* nindex, int* nlist, data_t* data1, data_t* data2, int n);
int verify_result(int* nindex, int* nlist, data_t* h_data1, data_t* h_data2, data_t* d_data1, data_t* d_data2, int n, int e, int blocks, int threadsperblock);

/****************************************************************************************/

int main (int argc, char* argv[])
{
  // process command line
  if (argc != 4) {fprintf(stderr, "USAGE: %s input_file_name threads_per_block num_blocks\n", argv[0]); exit(-1);}
  ECLgraph g = readECLgraph(argv[1]);
  int threadsPerBlock = atoi(argv[2]);
  if ((threadsPerBlock < 1) || (threadsPerBlock > 1024)) {fprintf(stderr, "ERROR: threads_per_block must be at least 1 and at most 1024\n"); exit(-1);}
  int numBlocks = atoi(argv[3]);
  if (numBlocks < 1) {fprintf(stderr, "ERROR: num_blocks must be at least 1\n"); exit(-1);}

  // allocate and init two data arrays
  int n = g.nodes;
  int e = g.edges;
  if (e == 0) {
    e = 1;
  }
  int s = ((n > e) ? n : e);
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

  // allocate and copy GPU data
  int* d_nindex;
  int* d_nlist;
  data_t* d_data1;
  data_t* d_data2;
  cudaMalloc((void**) &d_nindex, (n + 1) * sizeof(int));
  cudaMalloc((void**) &d_nlist, e * sizeof(int));
  cudaMalloc((void**) &d_data1, s * sizeof(data_t));
  cudaMalloc((void**) &d_data2, s * sizeof(data_t));
  cudaMemcpy(d_nindex, g.nindex, (n + 1) * sizeof(int), cudaMemcpyHostToDevice);
  cudaMemcpy(d_nlist, g.nlist, e * sizeof(int), cudaMemcpyHostToDevice);
  cudaMemcpy(d_data1, data1, s * sizeof(data_t), cudaMemcpyHostToDevice);
  cudaMemcpy(d_data2, data2, s * sizeof(data_t), cudaMemcpyHostToDevice);

  // run kernel
  test_kernel<<<numBlocks, threadsPerBlock>>>(d_nindex, d_nlist, d_data1, d_data2, n);

  // copy result back to CPU
  cudaMemcpy(data1, d_data1, s * sizeof(data_t), cudaMemcpyDeviceToHost);
  cudaMemcpy(data2, d_data2, s * sizeof(data_t), cudaMemcpyDeviceToHost);

  // check result
  serial_code(g.nindex, g.nlist, h_data1, h_data2, n);

  int ret = verify_result(g.nindex, g.nlist, h_data1, h_data2, data1, data2, n, e, numBlocks, threadsPerBlock);
  if (ret == 1) {
    printf("result matches serial code\n");
  } else if (ret == 0) {
    printf("result differs from serial code\n");
  }

  // cleanup
  cudaFree(d_nindex);
  cudaFree(d_nlist);
  cudaFree(d_data1);
  cudaFree(d_data2);
  free(data1);
  free(data2);
  free(h_data1);
  free(h_data2);
  freeECLgraph(&g);
  return 0;
}
