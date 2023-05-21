#pragma once

#ifndef graph_h
#define graph_h

#include <cstdint>
#include <vector>

using std::vector;

struct Graph {
    vector<vector<size_t>> edges;
    size_t edges_count = 0;

    explicit Graph(size_t n) : edges(n) {}
    void add_edge(size_t u, size_t v);
    size_t get_vertices_number() const;
};

#endif