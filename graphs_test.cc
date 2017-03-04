#include <iostream>
#include <vector>
#include "graphs.h"

void print(Graph g) {
  std::cout << g.to_string() << "\n";
}

int main() {
  DirectedGraph dg;
  print(dg);
}
