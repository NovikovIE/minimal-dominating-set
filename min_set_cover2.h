#ifndef min_set_cover_2_h
#define min_set_cover_2_h

#include <algorithm>
#include <set>
#include <unordered_map>
#include "min_edge_cover.h"

template <typename T>
bool cmp(const std::pair<size_t, vector<T>>& a, const std::pair<size_t, vector<T>>& b) {
    if (a.second.size() < b.second.size()) {
        return true;
    }
    if (a.second.size() > b.second.size()) {
        return false;
    }
    return std::lexicographical_compare(a.second.begin(), a.second.end(), b.second.begin(), b.second.end());
}

template <typename T>
using vset = std::set<std::pair<size_t, vector<T>>, decltype(cmp<T>)*>;

template <typename T>
std::set<vector<T>> get_min_set_cover_2(vset<T> sets, size_t elements_number) {
    Graph graph(elements_number);
    std::unordered_map<T, int> map;
    std::unordered_map<int, T> unmap;

    auto counter = 0;

    for (auto set : sets) {
        if (map.find(set.second[0]) == map.end()) {
            map[set.second[0]] = counter;
            unmap[counter++] = set.second[0];
        }
        if (map.find(set.second[1]) == map.end()) {
            map[set.second[1]] = counter;
            unmap[counter++] = set.second[1];
        }
        graph.add_edge(map[set.second[0]], map[set.second[1]]);
        graph.add_edge(map[set.second[1]], map[set.second[0]]);
    }

    auto edge_cover = std::move(get_min_edge_cover(graph));
    std::set<vector<T>> result;
    for (auto& edge : edge_cover) {
        result.insert({unmap[edge.first], unmap[edge.second]});
    }

    return std::move(result);
}

template <typename T = int>
std::set<size_t> get_min_set_cover_2(vset<T> sets, std::set<T> origin) {
    std::set<size_t> result;

    auto res = std::move(get_min_set_cover_2(sets, origin.size()));
    for (auto& el : sets) {
        if (res.find(el.second) != res.end() || res.find({el.second[1], el.second[0]}) != res.end()) {
            result.insert(el.first);
        }
    }
    return std::move(result);
}

#endif