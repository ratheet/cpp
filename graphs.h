#include <string>

using std::string;
using Value = std::pair<string, int>;

template<typename T>
concept bool Stringable = requires(T a) {
  { a.to_string() } -> string;
};
  
template<typename T>
concept bool Graph = Stringable<T>;
  /*&&
  //  requires(T a) {
};
  */

struct DirectedGraph {
  string to_string() {
    return "hello there!\n";
  }
};
