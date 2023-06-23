#ifndef blossom_h
#define blossom_h

#include <cstdint>
#include <queue>
#include <vector>
#include "graph.h"

struct Edge {
    int vertex = 0;
    Edge* next = nullptr;
};

class BlossomAlgo {
public:
    explicit BlossomAlgo(Graph& g);

    vector<std::pair<int, int>> find_matching();

private:
    void add_edge(int u, int v);
    int lca(int root, int u, int v);
    void mark_blossom(int lca, int u);
    void blossom_contraction(int s, int u, int v, std::queue<int>& q);
    int find_augmenting_path(int start);
    void augment_path(int vertex);

private:
    size_t sz = 0;

    vector<Edge> pool;
    vector<Edge>::iterator top;
    vector<Edge*> neighbour;

    vector<bool> is_in_queue;
    vector<bool> is_in_base;
    vector<vector<bool>> edges;

    vector<int> match;
    vector<int> parent;
    vector<int> base;
};

vector<std::pair<int, int>> get_max_matching(Graph& graph);

#endif