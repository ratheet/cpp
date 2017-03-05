#include <cassert>
#include <iostream>
#include <vector>
#include "graphs.h"

using std::cout;
using std::make_pair;

DirectedGraph make_unconnected_directed_graph() {
  DirectedGraph dg;
  Vertex v1(make_pair("A", 1));
  Vertex v2(make_pair("B", 2));
  Vertex v3(make_pair("C", 3));
  dg.add(v1);
  dg.add(v2);
  dg.add(v3);
  return dg;
}

template<typename Graph>
void print(Graph g) {
  cout << g.to_string() << "\n";
}

template<typename Graph>
int count_vertices(Graph g) {
  return g.vertex_count();
}

template<typename Graph>
int count_edges(Graph g) {
  return g.edge_count();
}

void test_print() {
  DirectedGraph dg = make_unconnected_directed_graph();
  print(dg);  
}

void test_adjacent() {
  DirectedGraph dg;
  Vertex v1(make_pair("A", 1));
  Vertex v2(make_pair("B", 2));
  Vertex v3(make_pair("C", 3));
  dg.add(v1);
  dg.add(v2);
  dg.add(v3);

  assert(!graph_lib::adjacent(dg,
			      std::make_unique<Vertex>(v1),
			      std::make_unique<Vertex>(v2)));
  assert(!graph_lib::adjacent(dg,
			      std::make_unique<Vertex>(v2),
			      std::make_unique<Vertex>(v1)));
  assert(!graph_lib::adjacent(dg,
			      std::make_unique<Vertex>(v1),
			      std::make_unique<Vertex>(v3)));
  assert(!graph_lib::adjacent(dg,
			      std::make_unique<Vertex>(v3),
			      std::make_unique<Vertex>(v1)));
  assert(!graph_lib::adjacent(dg,
			      std::make_unique<Vertex>(v2),
			      std::make_unique<Vertex>(v3)));
  assert(!graph_lib::adjacent(dg,
			      std::make_unique<Vertex>(v3),
			      std::make_unique<Vertex>(v2)));
}

void test_count_vertices() {
  DirectedGraph dg = make_unconnected_directed_graph();
  assert(count_vertices(dg) == 3);
}

void test_count_edges() {
  DirectedGraph dg = make_unconnected_directed_graph();
  assert(count_edges(dg) == 0);
}

void test_neighbors() {
  DirectedGraph dg;
  Vertex v1(make_pair("A", 1));
  Vertex v2(make_pair("B", 2));
  Vertex v3(make_pair("C", 3));
  dg.add(v1);
  dg.add(v2);
  dg.add(v3);

  assert(graph_lib::neighbors(dg,
			      std::make_unique<Vertex>(v1)).size() == 0);
  assert(graph_lib::neighbors(dg,
			      std::make_unique<Vertex>(v2)).size() == 0);
  assert(graph_lib::neighbors(dg,
			      std::make_unique<Vertex>(v3)).size() == 0);
}

void test_add() {
  DirectedGraph dg;
  Vertex v1(make_pair("A", 1));
  Vertex v2(make_pair("B", 2));
  Vertex v3(make_pair("C", 3));

  graph_lib::add(dg, v1);
  assert(count_vertices(dg) == 1);
}

int main() {
  assert(__cpp_concepts >= 201500); // check compiled with -fconcepts
  assert(__cplusplus >= 201500);    // check compiled with --std=c++1z

  cout << "Testing print.\n";
  test_print();
  cout << "Testing vertex count.\n";
  test_count_vertices();
  cout << "Test edge count.\n";
  test_count_edges();
  cout << "Testing adjacent().\n";
  test_adjacent();
  cout << "Testing neighbors().\n";
  test_neighbors();
  cout << "Testing add().\n";
  test_add();
  cout << "All tests passed.\n";
}

