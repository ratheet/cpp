#include <cassert>
#include <iostream>
#include <vector>
#include "graphs.h"

using std::cout;
using std::make_pair;

DirectedGraph make_directed_graph() {
  DirectedGraph dg;
  Vertex v1(make_pair("A", 1));
  Vertex v2(make_pair("B", 2));
  Vertex v3(make_pair("C", 3));
  dg.add(v1);
  dg.add(v2);
  dg.add(v3);
  return dg;
}

void print(Graph g) {
  cout << g.to_string() << "\n";
}

int count_vertices(Graph g) {
  return g.vertex_count();
}

void test_print() {
  DirectedGraph dg = make_directed_graph();
  print(dg);  
}

void test_count_vertices() {
  DirectedGraph dg = make_directed_graph();
  assert(count_vertices(dg) == 3);
}

int main() {
  assert(__cpp_concepts >= 201500); // check compiled with -fconcepts
  assert(__cplusplus >= 201500);    // check compiled with --std=c++1z

  test_print();
  test_count_vertices();
  cout << "All tests passed.\n";
}
