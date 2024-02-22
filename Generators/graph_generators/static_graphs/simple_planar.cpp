#include "graphGenerator.h"
#include "filter_nume.h"
#include <queue>
/**************************************************************************
g1: generate a simple planar graph in tree shape, the edge is from parent
to children and from left to right
***************************************************************************
g2: reversed-edge version of g1
***************************************************************************
g3: undirected version of g1
**************************************************************************/

struct Node
{
  int key;
  int id;
  Node* left;
  Node* right;
  Node* next_right;
};

Node* newNode(int k, int v)
{
  Node* newnode = new Node;
  newnode->key = k;
  newnode->id = v;
  newnode->left = newnode->right = newnode->next_right = NULL;
  return newnode;
}

Node* insert(Node* root, int k, int v)
{
  Node* newnode = newNode(k, v);
  Node* cur = root;
  Node* y = NULL;
  while (cur != NULL) {
    y = cur;
    if (k < cur->key) {
      cur = cur->left;
    } else {
      cur = cur->right;
    }
  }

  if (y == NULL) {
    y = newnode;
  } else if (k < y->key) {
    y->left = newnode;
  } else {
    y->right = newnode;
  }
  return y;
}

void connect(Node* root, int& m, std::set<int>* const edges1, std::set<int>* const edges2, std::set<int>* const edges3)
{
  std::queue<Node*> q;
  q.push(root);
  Node* temp = NULL;
  while (q.size() > 0)
  {
    int n = q.size();
    for (int i = 0; i < n; i++) {
      Node* prev = temp;
      temp = q.front();
      q.pop();
      if (i > 0) {
        prev->next_right = temp;
        int src = prev->id;
        int dst = -1;
        if (temp != NULL) {
          dst = temp->id;
        }
        edges1[src].insert(dst);
        edges2[dst].insert(src);
        edges3[src].insert(dst);
        edges3[dst].insert(src);
        m++;
      }
      if (temp->left != NULL) {
        q.push(temp->left);
      }
      if (temp->right != NULL) {
        q.push(temp->right);
      }
    }
    temp->next_right = NULL;
  }
}

int main(int argc, char* argv[])
{
  // process the command line
  if (argc < 4) {fprintf(stderr, "USAGE: %s number_of_vertices directed random_seed\n", argv[0]); exit(-1);}
  const int n = atoi(argv[1]);
  const int flag = atoi(argv[2]);
  if (n < 2) {fprintf(stderr, "ERROR: need at least 2 vertices\n"); exit(-1);}
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
  Node* root = NULL;
  root = insert(root, map[0], 0);
  for (int i = 1; i < n; i++) {
    Node* node = insert(root, map[i], i);
    int src = node->id;
    int dst = i;
    int parent_id = node->id;
    edges1[src].insert(dst);
    edges2[dst].insert(src);
    edges3[src].insert(dst);
    edges3[dst].insert(src);
    m++;
  }

  // connect all the nodes in the same level
  connect(root, m, edges1, edges2, edges3);

  if (filter_nume(m) && ((flag == 0) || (flag == 2))) {
    printf("\nPlanar graph\n");
    char name1[256];
    sprintf(name1, "simple_planar_%dn_%de.egr", n, m);
    saveAndPrint(n, m, name1, edges1);

    printf("\nCounter planar graph\n");
    char name2[256];
    sprintf(name2, "counter_simple_planar_%dn_%de.egr", n, m);
    saveAndPrint(n, m, name2, edges2);

  }

  if (filter_nume(m * 2) && ((flag == 0) || (flag == 1))) {
    printf("\nUndirected planar graph\n");
    char name3[256];
    sprintf(name3, "undirect_simple_planar_%dn_%de.egr", n, m * 2);
    saveAndPrint(n, m * 2, name3, edges3);
  }

  delete [] edges1;
  delete [] edges2;
  delete [] edges3;

  return 0;
}