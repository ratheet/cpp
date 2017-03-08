#include <cassert>
#include <iostream>
#include <typeinfo>
#include <vector>
#include "graphs.h"

using std::cout;
using std::make_pair;

DirectedGraph make_unconnected_directed_graph() {
  DirectedGraph dg;
  Vertex v1(make_pair("A", 1));
  Vertex v2(make_pair("B", 2));
  Vertex v3(make_pair("C", 3));

  dg.add(&v1);
  dg.add(&v2);
  dg.add(&v3);
  return dg;
}

void test_adjacent() {
  DirectedGraph dg;
  Vertex v1(make_pair("A", 1));
  Vertex v2(make_pair("B", 2));
  Vertex v3(make_pair("C", 3));
  Vertex* v1_ptr = std::make_unique<Vertex>(v1).get();
  Vertex* v2_ptr = std::make_unique<Vertex>(v2).get();
  Vertex* v3_ptr = std::make_unique<Vertex>(v3).get();

  dg.add(v1_ptr);
  dg.add(v2_ptr);
  dg.add(v3_ptr);
  
  assert(!graph_lib::adjacent(dg, v1_ptr, v2_ptr));
  assert(!graph_lib::adjacent(dg, v2_ptr, v1_ptr));
  assert(!graph_lib::adjacent(dg, v1_ptr, v3_ptr));
  assert(!graph_lib::adjacent(dg, v3_ptr, v1_ptr));
  assert(!graph_lib::adjacent(dg, v2_ptr, v3_ptr));
  assert(!graph_lib::adjacent(dg, v3_ptr, v2_ptr));
}

void print(Graph<Vertex*> g) {
  cout << g.to_string() << "\n";
}

int count_vertices(Graph<Vertex*> g) {
  return g.vertex_count();
}

int count_edges(Graph<Vertex*> g) {
  return g.edge_count();
}

void test_print() {
  DirectedGraph dg = make_unconnected_directed_graph();
  print(dg);  
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
  Vertex* v1_ptr = std::make_unique<Vertex>(v1).get();
  Vertex* v2_ptr = std::make_unique<Vertex>(v2).get();
  Vertex* v3_ptr = std::make_unique<Vertex>(v3).get();

  dg.add(v1_ptr);
  dg.add(v2_ptr);
  dg.add(v3_ptr);
  
  assert(graph_lib::neighbors(dg, v1_ptr).size() == 0);
  assert(graph_lib::neighbors(dg, v2_ptr).size() == 0);
  assert(graph_lib::neighbors(dg, v3_ptr).size() == 0);
}

void test_add() {
  DirectedGraph dg;
  Vertex v1(make_pair("A", 1));
  Vertex v2(make_pair("B", 2));
  Vertex v3(make_pair("C", 3));

  graph_lib::add(dg, &v1);
  assert(count_vertices(dg) == 1);
  // TODO: assert v1 in graph
  }

void test_remove() {
  DirectedGraph dg;
  Vertex v1(make_pair("A", 1));
  Vertex v2(make_pair("B", 2));
  Vertex v3(make_pair("C", 3));
  dg.add(&v1);
  dg.add(&v2);
  dg.add(&v3);

  assert(count_vertices(dg) == 3);
  graph_lib::remove(dg, &v1);
  assert(count_vertices(dg) == 2);
  // TODO: assert v1 no longer in graph
}

int main() {
  assert(__cpp_concepts >= 201500); // check compiled with -fconcepts
  assert(__cplusplus >= 201500);    // check compiled with --std=c++1z

  cout << "Testing adjacent().\n";
  test_adjacent();
  cout << "Testing neighbors().\n";
  test_neighbors();
  cout << "Testing add().\n";
  test_add();
  cout << "Testing remove().\n";
  test_remove();
  cout << "Testing print.\n";
  test_print();
  cout << "Testing vertex count.\n";
  test_count_vertices();
  cout << "Test edge count.\n";
  test_count_edges();

  cout << "All tests passed.\n";
}

