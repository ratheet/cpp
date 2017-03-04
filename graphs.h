#include <sstream>
#include <string>
#include <vector>

using std::ostringstream; 
using std::string;
using std::vector;
using Value = std::pair<string, int>;

template<typename T>
concept bool Stringable = requires(T a) {
  { a.to_string() } -> string;
};

template<typename T>
concept bool Vertex_ptr = Stringable<T>;
  
template<typename G>
concept bool Graph =
  Stringable<G> &&
  requires(G g) {
  { g.vertex_count() } -> int;
};

class Vertex {
 public:
  Vertex(Value value) : value_(value) {}
  string to_string() const {
    ostringstream oss;
    oss << "(" << this->value_.first << ", " << this->value_.second << ")";
    return oss.str();
  }
  
 private:
  Value value_;
};

template<Vertex_ptr V>
class DirectedGraph {
 public:
  string to_string() const {
    string str_value = "Graph:\n";
    for (const Vertex& v : vertices_) {
      return str_value += v.to_string();
    }
    return str_value;
  }

  void add(V v) {
    vertices_.push_back(v);
  }
  
  int vertex_count() {
    return vertices_.size();
  }
  
 private:
  vector<V> vertices_;
};

