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

void print(Graph<Vertex*, Edge*> g) {
  cout << g.to_string() << "\n";
}

int count_vertices(Graph<Vertex*, Edge*> g) {
  return g.vertex_count();
}

int count_edges(Graph<Vertex*, Edge*> g) {
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
  assert(dg.vertex_count() == 1);
  // TODO: assert v1 in graph
}

void test_add_edge() {
  DirectedGraph dg;
  Vertex v1(make_pair("A", 1));
  Vertex v2(make_pair("B", 2));
  Vertex v3(make_pair("C", 3));

  graph_lib::add_edge(dg, &v1, &v2);
  graph_lib::add_edge(dg, &v1, &v3);

  assert(dg.edge_count() == 2);

  Edge e1(std::make_unique<Vertex>(v1),
	  std::make_unique<Vertex>(v2),
	  std::make_unique<Value>(kDummyValue));
  graph_lib::add_edge(dg, &e1);

  assert(dg.edge_count() == 3);
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

void test_value() {
  DirectedGraph dg;
  Value val1 = make_pair("A", 1);
  Value val2 = make_pair("B", 2);
  Value val3 = make_pair("C", 3);
  Vertex v1(val1);
  Vertex v2(val2);
  Vertex v3(val3);
  dg.add(&v1);
  dg.add(&v2);
  dg.add(&v3);

  assert(val1 == graph_lib::value(&v1));
  assert(val2 == graph_lib::value(&v2));
  assert(val3 == graph_lib::value(&v3));
}

void test_set_value() {
  DirectedGraph dg;
  Value val1 = make_pair("A", 1);
  Value val2 = make_pair("B", 2);
  Value val3 = make_pair("C", 3);
  Vertex v1(val1);
  Vertex v2(val2);
  Vertex v3(val3);
  dg.add(&v1);
  dg.add(&v2);
  dg.add(&v3);

  // Cycle the values between vertices.
  graph_lib::set_value(&v1, val2);
  graph_lib::set_value(&v2, val3);
  graph_lib::set_value(&v3, val1);
  assert(val2 == v1.value());
  assert(val3 == v2.value());
  assert(val1 == v3.value());
}

void test_edge_value() {
  Value val1 = make_pair("A", 1);
  Value val2 = make_pair("B", 2);
  Value val3 = make_pair("C", 3);
  Vertex v1(val1);
  Vertex v2(val2);
 
  Edge e = Edge(std::make_unique<Vertex>(v1),
		 std::make_unique<Vertex>(v2),
		 std::make_unique<Value>(val3));
  assert(val3 == graph_lib::value(&e));
}

void test_set_edge_value() {
  Value val1 = make_pair("A", 1);
  Value val2 = make_pair("B", 2);
  Value val3 = make_pair("C", 3);
  Vertex v1(val1);
  Vertex v2(val2);
 
  Edge e = Edge(std::make_unique<Vertex>(v1),
		std::make_unique<Vertex>(v2),
		std::make_unique<Value>(val3));
  assert(val3 == graph_lib::value(&e));
  graph_lib::set_value(&e, val2);
  assert(val2 == *e.value());
}

void test_top() {
DirectedGraph dg;
  Value val1 = make_pair("A", 1);
  Value val2 = make_pair("B", 2);
  Value val3 = make_pair("C", 3);
  Vertex v1(val1);
  Vertex v2(val2);
  Vertex v3(val3);
  dg.add(&v1);
  dg.add(&v2);
  dg.add(&v3);

  assert(v1 == *graph_lib::top(dg));
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
  cout << "Testing add_edge().\n";
  test_add_edge();
  cout << "Testing remove().\n";
  test_remove();
  cout << "Testing print.\n";
  test_print();
  cout << "Testing vertex count.\n";
  test_count_vertices();
  cout << "Test edge count.\n";
  test_count_edges();
  cout << "Testing value().\n";
  test_value();
  cout << "Testing set_value().\n";
  test_set_value();
  cout << "Testing set_edge_value().\n";
  test_set_edge_value();
  cout << "Testing top().\n";
  test_top();
  cout << "All tests passed.\n";
}

