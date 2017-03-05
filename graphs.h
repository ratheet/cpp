#include <memory>
#include <sstream>
#include <string>
#include <vector>

using std::ostringstream; 
using std::string;
using std::unique_ptr;
using std::vector;
using Value = std::pair<string, int>;

// Forward-declare Vertex class for use by Vertex_ptr concept.
class Vertex;

// Concept definitions.
template<typename S>
concept bool Stringable = requires(S s) {
  { s.to_string() } -> string;
};

template<typename V>
concept bool Vertex_ptr =
  Stringable<V> &&
  requires(V v) {
  { v.get() } -> Vertex*;
};

template<typename E>
concept bool Edge_ptr = Stringable<E>;

template<typename G, typename U, typename V>
concept bool Graph =
  Stringable<G> &&
  Vertex_ptr<U> &&
  Vertex_ptr<V> &&
  requires(G g, U u, V v) {
  { g.vertex_count() } -> int;
  { g.edge_count() } -> int;
  { g.are_adjacent(u, v) } -> bool;
  { g.get_neighbors(u) } -> std::vector<Vertex_ptr>;
  { g.add(u) } -> void;
};

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
  string to_string() const {
    ostringstream oss;
    oss << "(" << this->value_.first << ", " << this->value_.second << ")";
    return oss.str();
  }
  
 private:
  Value value_;
};

class Edge {
 public:
  Edge() {}
  Edge(unique_ptr<Vertex> source, unique_ptr<Vertex> dest) noexcept
    : source_(std::move(source)), dest_(std::move(dest)) {}
  Edge(Edge&& edge) noexcept
    : source_(std::move(edge.source_)), dest_(std::move(edge.dest_)) {
  }
  Edge(const Edge &edge) noexcept {
    if (edge.source_) {
      source_ = std::make_unique<Vertex>(*(edge.source_.get()));
    }
    if (edge.dest_) {
      dest_ = std::make_unique<Vertex>(*(edge.dest_.get()));
    }
  }
  ~Edge() noexcept {}
  Edge& operator=(Edge &&edge) noexcept {
    if (this != &edge) {
      source_ = std::move(edge.source_);
      dest_ = std::move(edge.dest_);
    }
    return *this;
  }
  Edge& operator=(const Edge&) noexcept = delete;

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
  
 private:
  unique_ptr<Vertex> source_;
  unique_ptr<Vertex> dest_;
};

class DirectedGraph {
 public:
  string to_string() const {
    string str_value = "Graph (# vertices = " + std::to_string(vertex_count()) + "):\n";
    for (const Edge& e : edges_) {
      str_value += e.to_string() + "\n";
    }
    return str_value;
  }

  void add(const Vertex& v) {
    Edge edge;
    edge.set_source(v);
    edges_.push_back(edge);
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

  bool are_adjacent(const unique_ptr<Vertex>& u,
		    const unique_ptr<Vertex>& v) const {
    for (const Edge& e : edges_) {
      if (e.get_source() && *(e.get_source().get()) == *(u.get())) {
	if (e.get_dest() && *(e.get_dest().get()) == *(v.get())) {
	  return true;
	}
      }
    }
    return false;
  }
  
  vector<unique_ptr<Vertex>> get_neighbors(const unique_ptr<Vertex>& vertex)
      const {
    vector<unique_ptr<Vertex>> neighbors;
    for (const Edge& e : edges_) {
      if (e.get_source() && *(e.get_source()) == *vertex.get()) {
	if (e.get_dest()) {
	  neighbors
	    .push_back(std::make_unique<Vertex>(*(e.get_dest().get())));
	}
      } else if (e.get_dest() && *(e.get_dest()) == *(vertex.get())) {
	if (e.get_source()) {
	  neighbors
	    .push_back(std::make_unique<Vertex>(*(e.get_source().get())));
	}
      }      
    }
    return neighbors;
  }

 private:
  vector<Edge> edges_;
};

// Library functions using concepts.
namespace graph_lib {
  template<typename Graph, typename Vertex_ptr>
    bool adjacent(Graph& g, Vertex_ptr x, Vertex_ptr y) {
    return g.are_adjacent(x, y);
  }

  template<typename Graph, typename Vertex_ptr>
    vector<Vertex_ptr> neighbors(Graph& g, Vertex_ptr x) {
    return g.get_neighbors(x);
  }

  template<typename Graph, typename Vertex_ptr>
    void add(Graph& g, Vertex_ptr x) {
    std::cout << "Adding...\n";
    g.add(x);
  }
} // namespace graph_lib
