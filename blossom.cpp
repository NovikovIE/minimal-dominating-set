#include "blossom.h"

BlossomAlgo::BlossomAlgo(Graph& g)
    : sz(g.get_vertices_number()),
      pool(sz * sz * 2),
      top(pool.begin()),
      neighbour(sz),
      is_in_queue(sz, false),
      is_in_base(sz, false),
      edges(sz, vector<bool>(sz, false)),
      match(sz, 0),
      parent(sz, 0),
      base(sz, 0) {
    for (auto i = 0; i < g.get_vertices_number(); ++i) {
        for (auto j = 0; j < g.edges[i].size(); ++j) {
            auto v = g.edges[i][j];
            add_edge(i, v);
        }
    }
}

vector<std::pair<int, int>> BlossomAlgo::find_matching() {
    match.assign(match.size(), -1);
    for (auto vertex = 0; vertex < sz; ++vertex) {
        if (match[vertex] == -1) {
            augment_path(find_augmenting_path(vertex));
        }
    }
    vector<std::pair<int, int>> result;
    for (int i = 0; i < sz; ++i) {
        if (i < match[i]) {
            result.emplace_back(i, match[i]);
        }
    }
    return std::move(result);
}

void BlossomAlgo::add_edge(int u, int v) {
    if (!edges[u][v]) {
        top->vertex = v;
        top->next = neighbour[u];
        neighbour[u] = &*(top++);
        top->vertex = u;
        top->next = neighbour[v];
        neighbour[v] = &*(top++);
        edges[u][v] = edges[u][v] = true;
    }
}

int BlossomAlgo::lca(int root, int u, int v) {
    vector<bool> inp(sz, false);
    while (true) {
        inp[u = base[u]] = true;
        if (u == root) {
            break;
        }
        u = parent[match[u]];
    }
    while (true) {
        if (inp[v = base[v]]) {
            return v;
        }
        v = parent[match[v]];
    }
}

void BlossomAlgo::mark_blossom(int lca, int u) {
    while (base[u] != lca) {
        int v = match[u];
        is_in_base[base[u]] = is_in_base[base[v]] = true;
        u = parent[v];

        if (base[u] != lca) {
            parent[u] = v;
        }
    }
}

void BlossomAlgo::blossom_contraction(int s, int u, int v, std::queue<int>& q) {
    int lca_res = lca(s, u, v);
    is_in_base.assign(is_in_base.size(), false);

    mark_blossom(lca_res, u);
    mark_blossom(lca_res, v);

    if (base[u] != lca_res) {
        parent[u] = v;
    }

    if (base[v] != lca_res) {
        parent[v] = u;
    }

    for (auto k = 0; k < sz; ++k) {
        if (is_in_base[base[k]]) {
            base[k] = lca_res;
            if (!is_in_queue[k]) {
                q.push(k);
                is_in_queue[k] = true;
            }
        }
    }
}

int BlossomAlgo::find_augmenting_path(int start) {
    is_in_queue.assign(is_in_queue.size(), false);
    parent.assign(parent.size(), -1);

    for (int i = 0; i < sz; ++i) {
        base[i] = i;
    }

    std::queue<int> q;
    q.push(start);

    is_in_queue[start] = true;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (Edge* e = neighbour[u]; e; e = e->next) {
            int v = e->vertex;
            if (base[u] != base[v] && match[u] != v) {
                if ((v == start) || (match[v] != -1 && parent[match[v]] != -1)) {
                    blossom_contraction(start, u, v, q);
                } else if (parent[v] == -1) {
                    parent[v] = u;
                    if (match[v] == -1) {
                        return v;
                    }
                    if (!is_in_queue[match[v]]) {
                        q.push(match[v]);
                        is_in_queue[match[v]] = true;
                    }
                }
            }
        }
    }
    return -1;
}

void BlossomAlgo::augment_path(int vertex) {
    int u = 0;
    while (vertex != -1) {
        u = match[parent[vertex]];
        match[parent[vertex]] = vertex;
        match[vertex] = parent[vertex];
        vertex = u;
    }
}

vector<std::pair<int, int>> get_max_matching(Graph& graph) {
    BlossomAlgo blossom(graph);
    return std::move(blossom.find_matching());
}
