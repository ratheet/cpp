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
  vector<Edge> get_adjacency_list() {
    return directed_graph_.get()->get_adjacency_list();
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
