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
