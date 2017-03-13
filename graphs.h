#include <algorithm>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

using std::ostringstream; 
using std::string;
using std::unique_ptr;
using std::vector;
using Value = std::pair<string, int>;

const Value kDummyValue = std::pair<string, int>("DUMMY", -1);

// Forward-declarations.
class Edge;
class Vertex;

// Concept definitions.
template<typename S>
concept bool Stringable = requires(S s) {
  { s.to_string() } -> string;
};

template<typename V>
concept bool Vertex_ptr = requires(V v, Value w) {
  { *v } -> const Vertex&;
  { v->value() } -> Value&;
  { v->set_value(w) } -> void;
};

template<typename E>
concept bool Edge_ptr = requires(E e, Value v) {
  { *e } -> const Edge&;
  { e->value() } -> Value*;
  { e->set_value(v) } -> void;
};

template<typename G, typename V, typename E>
concept bool Graph = 
  Stringable<G> &&
  Vertex_ptr<V> &&
  Edge_ptr<E> &&
  requires(G&& g, V u, V v, E e) {
  { g.add(u) } -> void;
  { g.add_edge(u, v) } -> bool;
  { g.add_edge(e) } -> bool;
  { g.are_adjacent(u, v) } -> bool;
  { g.edge_count() } -> int;
  { g.get_neighbors(u) } -> std::vector<V>;
  { g.remove(u) } -> void;
  { g.top() } -> V;
  { g.vertex_count() } -> int;
};

// Library functions using concepts.
namespace graph_lib {
  bool adjacent(Graph<Vertex*, Edge*>&& g, Vertex_ptr& u, Vertex_ptr& v) {
    return g.are_adjacent(u, v);
  }
  
  vector<Vertex*> neighbors(Graph<Vertex*, Edge*>& g, Vertex_ptr x) {
    return g.get_neighbors(x);
  }
  
  void add(Graph<Vertex*, Edge*>& g, Vertex_ptr x) {
    g.add(x);
  }

  void remove(Graph<Vertex*, Edge*>& g, Vertex_ptr x) {
    g.remove(x);
  }

  bool add_edge(Graph<Vertex*, Edge*>& g, Vertex_ptr x, Vertex_ptr y) {
    return g.add_edge(x, y);
  }

  bool add_edge(Graph<Vertex*, Edge*>& g, Edge_ptr x) {
    return g.add_edge(x);
  }

  Value value(Vertex_ptr x) {
    return x->value();
  }

  void set_value(Vertex_ptr x, Value v) {
    x->set_value(v);
  }

  Value value(Edge_ptr e) {
    return *e->value();
  }

  void set_value(Edge_ptr e, Value v) {
    e->set_value(v);
  }

  Vertex_ptr top(Graph<Vertex*, Edge*>& g) {
    return g.top();
  }
}

// Class definitions.
class Vertex {
 public:
  Vertex(const Vertex& vertex) : value_(vertex.value_) {}
  Vertex(const Value value) : value_(value) {}
  bool operator==(const Vertex& other) const {
    if (this == &other) {
      return true;
    }
    return this->value_ == other.value_;
  }
  bool operator!=(const Vertex& other) const {
    if (this == &other) {
      return false;
    }
    return this->value_ != other.value_;
  }
  string to_string() const {
    ostringstream oss;
    oss << "(" << this->value_.first << ", " << this->value_.second << ")";
    return oss.str();
  }
  Value& value() {
    return value_;
  }
  void set_value(Value& value) {
    value_ = value;
  }

 private:
  Value value_;
};

class Edge {
 public:
  Edge() {}
  Edge(unique_ptr<Vertex> source, unique_ptr<Vertex> dest, unique_ptr<Value> value) noexcept
    : source_(std::move(source)), dest_(std::move(dest)), value_(std::move(value)) {}
  Edge(Edge&& edge) noexcept
    : source_(std::move(edge.source_)), dest_(std::move(edge.dest_)), value_(std::move(edge.value_)) {}
  Edge(const Edge &edge) noexcept { 
    if (edge.source_) {
      source_ = std::make_unique<Vertex>(*(edge.source_.get()));
    }
    if (edge.dest_) {
      dest_ = std::make_unique<Vertex>(*(edge.dest_.get()));
    }
    if (edge.value_) {
      value_ = std::make_unique<Value>(*(edge.value_.get()));
    }
  }
  ~Edge() noexcept {}
  Edge& operator=(Edge &&edge) noexcept {
    if (this != &edge) {
      source_ = std::move(edge.source_);
      dest_ = std::move(edge.dest_);
      value_ = std::move(edge.value_);
    }
    return *this;
  }
  Edge& operator=(const Edge&) noexcept = delete;
  bool operator==(const Edge& other) const {
    if (this == &other) {
      return true;
    }
    if (this->source_ && *(this->source_.get()) == *(other.source_.get())) {
      if (this->dest_ && *(this->dest_.get()) == *(other.dest_.get())) {
	if (this->value_ && *(this->value_.get()) == *(other.value_.get())) {
	  return true;
	}
      }
    }
    return false;
  }

  void set_source(const Vertex& v) {
    source_ = std::make_unique<Vertex>(v);
  }

  void set_dest(const Vertex& v) {
    dest_ = std::make_unique<Vertex>(v);
  }
  
  string to_string() const {
    ostringstream oss;
    if (source_) {
      oss << source_->to_string();
    } else {
      oss << "NULL";
    }
    oss << " -> ";
    if (dest_) {
      oss << dest_->to_string();
    } else {
      oss << "NULL";
    }
    oss << "\n";
    return oss.str();
  }

  const unique_ptr<Vertex>& get_source() const {
    return source_;
  }

  const unique_ptr<Vertex>& get_dest() const {
    return dest_;
  }

  Value* value() {
    return value_.get();
  }

  void set_value(Value& value) {
    value_ = std::make_unique<Value>(value);
  }

 private:
  unique_ptr<Vertex> source_;
  unique_ptr<Vertex> dest_;
  unique_ptr<Value> value_ = std::make_unique<Value>(kDummyValue);
};

class DirectedGraph {
 public:
  void add(const Vertex* v) {
    Edge edge;
    edge.set_source(*v);
    edges_.push_back(edge);
  }

  bool add_edge(const Vertex* u, const Vertex* v) {
    Edge edge;
    edge.set_source(*u);
    edge.set_dest(*v);
    edges_.push_back(edge);
    return true;
  }

  bool add_edge(const Edge* e) {
    edges_.push_back(*e);
    return true;
  }

  void remove_edge(const Edge* e) {
    edges_.erase(std::remove(edges_.begin(), edges_.end(), *e), edges_.end());
  }

  bool are_adjacent(const Vertex* u, const Vertex* v) {
    for (const Edge& e : edges_) {
      if (e.get_source().get() && *(e.get_source().get()) == *u) {
	if (e.get_dest().get() && *(e.get_dest().get()) == *v) {
	  return true;
	}
      }
    }
    return false;
  }

  int edge_count() const {
    int num_edges = 0;
    for (const Edge& e : edges_) {
      // An Edge is considered a "true" edge only if it has both a
      // source and a destination.
      if (e.get_source() && e.get_dest()) {
	num_edges++;
      }
    }
    return num_edges;
  }

  vector<Edge> get_adjacency_list() {
    return edges_;
  }

  vector<Vertex_ptr> get_neighbors(Vertex_ptr& vertex)
      const {
    vector<Vertex*> neighbors;
    for (const Edge& e : edges_) {
      if (e.get_source() && *(e.get_source()) == *vertex) {
	if (e.get_dest()) {
	  neighbors
	    .push_back(std::make_unique<Vertex>(*(e.get_dest().get())).get());
						
	}
      } else if (e.get_dest() && *(e.get_dest()) == *vertex) {
	if (e.get_source()) {
	  neighbors
	    .push_back(std::make_unique<Vertex>(*(e.get_source().get())).get());
	}
      }      
    }
    return neighbors;
  }

  void remove(const Vertex* v) {
    for (Edge& e : edges_) {
      if (e.get_source().get() && *(e.get_source().get()) == *v) {
	// Remove the edge: if the source is gone, the edge to the dest
	// is no longer needed.
	auto it = std::find(edges_.begin(), edges_.end(), e);
	if (it != edges_.end()) {
	  edges_.erase(it);
	}
      }
    }
  }

  string to_string() const {
    string str_value = "Graph (# vertices = " + std::to_string(vertex_count()) + "):\n";
    for (const Edge& e : edges_) {
      str_value += e.to_string() + "\n";
    }
    return str_value;
  }

  Vertex* top() {
    for (const Edge& e : edges_) {
      if (e.get_source().get()) {
	return e.get_source().get();
      }
    }
    return nullptr;
  }
  
  int vertex_count() const {
    int num_vertices = 0;
    for (const Edge& e : edges_) {
      if (e.get_source().get()) {
	num_vertices++;
      }
      if (e.get_dest().get()) {
	num_vertices++;
      }
    }
    return num_vertices;
  }

 private:
  vector<Edge> edges_;
};

class DirectedAcyclicGraph {
 public:
  DirectedAcyclicGraph() {
    directed_graph_ = std::make_unique<DirectedGraph>();
  }
  void add(const Vertex* u) {
    directed_graph_.get()->add(u);
  }
  bool add_edge(const Vertex* source, const Vertex* dest) {
    directed_graph_.get()->add_edge(source, dest);
    if (check_for_cycles_()) {
      Edge edge(std::make_unique<Vertex>(*source), std::make_unique<Vertex>(*dest), std::make_unique<Value>(kDummyValue));
      directed_graph_.get()->remove_edge(&edge);
      return false;
    }
  }
  bool add_edge(const Edge* edge) {
    directed_graph_.get()->add_edge(edge);
    if (check_for_cycles_()) {
      directed_graph_.get()->remove_edge(edge);
      return false;
    }
  }
  bool are_adjacent(const Vertex* u, const Vertex* v) {
    return directed_graph_.get()->are_adjacent(u, v);
  }
  int edge_count() {
    return directed_graph_.get()->edge_count();
  }
  vector<Vertex*> get_neighbors(Vertex* u) {
    return directed_graph_.get()->get_neighbors(u);
  }
  void remove(const Vertex* u) {
    directed_graph_.get()->remove(u);
  }
  Vertex* top() {
    return directed_graph_.get()->top();
  }
  int vertex_count() {
    return directed_graph_.get()->vertex_count();
  }
  string to_string() {
    return directed_graph_.get()->to_string();
  }
 private:
  unique_ptr<DirectedGraph> directed_graph_;

  bool check_for_cycles_() {
    // Based on http://www.geeksforgeeks.org/detect-cycle-in-a-graph/.
    std::map<int, Vertex*> vertex_ids_to_ptrs;
    std::map<int, bool> recursive_stack;
    for (const Edge& e : directed_graph_.get()->get_adjacency_list()) {
      if (e.get_source().get()) {
	// the second part of a Value is its ID.
	vertex_ids_to_ptrs.insert(std::pair<int, Vertex*>(e.get_source().get()->value().second, e.get_source().get()));
	recursive_stack.insert(std::pair<int, bool>(e.get_source().get()->value().second, false));
      }
      if (e.get_dest().get()) {
	vertex_ids_to_ptrs.insert(std::pair<int, Vertex*>(e.get_dest().get()->value().second, e.get_dest().get()));
	recursive_stack.insert(std::pair<int, bool>(e.get_dest().get()->value().second, false));
      }
    }
    std::set<int> visited_set;
    for (auto& vertex_id_to_ptr : vertex_ids_to_ptrs) {
      cycle_checker_(vertex_ids_to_ptrs, vertex_id_to_ptr.first, visited_set, recursive_stack);
    }
    return false;
  }
  bool cycle_checker_(std::map<int, Vertex*>& vertex_id_to_ptrs, int vertex_id, std::set<int>& visited_set, std::map<int, bool>& recursive_stack) {
    if (visited_set.find(vertex_id) == visited_set.end()) {
      visited_set.insert(vertex_id);
      recursive_stack[vertex_id] = true;
      Vertex* vtx = vertex_id_to_ptrs[vertex_id];
      vector<Vertex*> neighbors = directed_graph_.get()->get_neighbors(vtx);
      for (Vertex* neighbor : neighbors) {
	int neighbor_id = neighbor->value().second;
	if (visited_set.find(neighbor_id) == visited_set.end() && cycle_checker_(vertex_id_to_ptrs, neighbor_id, visited_set, recursive_stack)) {
	  return true;
	} else if (recursive_stack[neighbor_id]) {
	  return true;
	}
      }
    }
    recursive_stack[vertex_id] = false;
    return false;
  }
};

class Tree {
 public:
  Tree() {
    dag_ = std::make_unique<DirectedAcyclicGraph>();
  }
  void add(const Vertex* u) {
    dag_.get()->add(u);
  }
  bool add_edge(const Vertex* source, const Vertex* dest) {
    return dag_.get()->add_edge(source, dest);
  }
  bool add_edge(const Edge* edge) {
    return dag_.get()->add_edge(edge);
  }  
  bool are_adjacent(const Vertex* u, const Vertex* v) {
    return dag_.get()->are_adjacent(u, v);
  }
  int edge_count() {
    return dag_.get()->edge_count();
  }
  vector<Vertex*> get_neighbors(Vertex* u) {
    return dag_.get()->get_neighbors(u);
  }
  void remove(const Vertex* u) {
    dag_.get()->remove(u);
  }
  Vertex* top() {
    // TODO: top should be root
    return dag_.get()->top();
  }
  int vertex_count() {
    return dag_.get()->vertex_count();
  }
  string to_string() {
    return dag_.get()->to_string();
  }

 private:
  unique_ptr<DirectedAcyclicGraph> dag_;
};
