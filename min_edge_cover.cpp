#include "min_edge_cover.h"

vector<std::pair<int, int>> get_min_edge_cover(Graph& graph) {
    auto matching = std::move(get_max_matching(graph));
    vector<bool> covered_vertices(graph.get_vertices_number(), false);
    for (auto& par : matching) {
        covered_vertices[par.first] = covered_vertices[par.second] = true;
    }
    auto result = std::move(matching);
    for (auto i = 0; i < covered_vertices.size(); ++i) {
        if (!covered_vertices[i]) {
            if (graph.edges[i].empty()) {
                std::cout << "No Edge Cover\n";
                exit(1);
            }
            covered_vertices[i] = covered_vertices[graph.edges[i][0]] = true;
            result.push_back({i, graph.edges[i][0]});
        }
    }
    return result;
}
