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
