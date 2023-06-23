#ifndef MINIMAL_DOMINATING_SET_MINIMAL_DOMINATING_SET_H
#define MINIMAL_DOMINATING_SET_MINIMAL_DOMINATING_SET_H

#include "min_set_cover.h"

vector<size_t> get_mds(Graph graph) {
    vector<size_t> set(graph.get_vertices_number());
    std::set<vector<size_t>> sets;
    for (auto i = 0; i < set.size(); ++i) {
        set[i] = i;
    }
    for (size_t i = 0; i < graph.edges.size(); ++i) {
        vector<size_t> temp;
        temp.push_back({i});
        for (auto to : graph.edges[i]) {
            temp.push_back({to});
        }
        sets.insert(std::move(temp));
    }
    vector<vector<size_t>> vc(sets.begin(), sets.end());
    auto [unused, set_cover_numbers] = get_msc_numbers(std::move(vc), std::move(set));
    return std::move(std::vector<size_t>(set_cover_numbers.begin(), set_cover_numbers.end()));
}

#endif  //MINIMAL_DOMINATING_SET_MINIMAL_DOMINATING_SET_H
