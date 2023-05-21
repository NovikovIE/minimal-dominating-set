#include "graph.h"

void Graph::add_edge(size_t u, size_t v) {
    edges[u].push_back(v);
    ++edges_count;
}

size_t Graph::get_vertices_number() const {
    return edges.size();
}
