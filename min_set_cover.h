#pragma once

#ifndef min_set_cover_h
#define min_set_cover_h

#include <set>
#include <vector>
#include "min_set_cover2.h"

struct Metrics {
    size_t del_count = 0;
    size_t msc_count = 0;
    size_t set_erase_count = 0;
    size_t set_get_count = 0;

    void clear() {
        del_count = 0;
        msc_count = 0;
        set_erase_count = 0;
        set_get_count = 0;
    }

    void print() const {
        std::cout << "del_count : " << del_count << "\n";
        std::cout << "msc_count : " << msc_count << "\n";
        std::cout << "set_erase_count : " << set_erase_count << "\n";
        std::cout << "set_get_count : " << set_get_count << "\n";
    }

    vector<size_t> get_metrics() {
        return {del_count, msc_count, set_erase_count, set_get_count};
    }
};

static Metrics metrics;

template <typename T>
std::pair<vset<T>, std::set<T>> del(vset<T> sets, vector<T> x, std::set<T> origin) {
    ++metrics.del_count;

    std::set<T> x_set(x.begin(), x.end());
    vset<T> result(cmp<T>);
    for (auto& subset : sets) {
        vector<T> new_subset;
        for (auto& val : subset.second) {
            if (x_set.find(val) == x_set.end()) {
                new_subset.push_back(std::move(val));
            }
        }
        if (!new_subset.empty()) {
            result.template emplace(subset.first, std::move(new_subset));
        }
    }
    for (auto& el : x) {
        origin.erase(el);
    }
    return {std::move(result), std::move(origin)};
}

template <typename Iterator>
bool includes(Iterator first1, Iterator last1, Iterator first2, Iterator last2) {
    while (first2 != last2) {
        if (first1 == last1) {
            return false;
        }
        if (*first1 == *first2) {
            ++first2;
        } else {
            ++first1;
        }
    }
    return true;
}

template <typename T>
std::set<size_t> min_set_cover(vset<T> sets, std::set<T> origin) {
    ++metrics.msc_count;

    if (sets.size() == 0) {
        return {};
    }

    // if exists Y subset X in sets
    for (auto it1 = sets.rbegin(); it1 != sets.rend(); ++it1) {
        auto it2 = it1;
        ++it2;
        for (; it2 != sets.rend(); ++it2) {
            auto& X = *it1;
            auto& Y = *it2;
            if (includes(X.second.begin(), X.second.end(), Y.second.begin(), Y.second.end())) {  // Y is a subset
                ++metrics.set_erase_count;

                sets.erase(*it2);
                return std::move(min_set_cover(std::move(sets), std::move(origin)));
            }
        }
    }

    // if exists v in set that contained only in one set
    {
        vector<size_t> count(origin.size(), 0);
        vector<int> set_num(origin.size(), -1);
        std::unordered_map<T, size_t> map;
        size_t counter = 0;
        auto set_number = 0;
        for (auto& set : sets) {
            for (auto& el : set.second) {
                if (map.find(el) == map.end()) {
                    map[el] = counter++;
                }
                auto temp = map[el];
                ++count[temp];
                set_num[temp] = set_number;
            }
            ++set_number;
        }

        for (auto i = 0; i < origin.size(); ++i) {
            if (count[i] == 1) {
                ++metrics.set_get_count;

                auto iter = sets.begin();
                std::advance(iter, set_num[i]);
                auto X = *iter;
                auto del_res = std::move(del(std::move(sets), std::move(X.second), std::move(origin)));
                auto res = std::move(min_set_cover(std::move(del_res.first), std::move(del_res.second)));
                res.insert(X.first);
                return std::move(res);
            }
        }
    }

    auto max_set = *(--sets.end());
    // if |max_set| = 2 then all sets are of cardinality 2,
    // because otherwise there sets of cardinality 1,
    // but this can't be because they are then either subsets of sets of cardinality 2
    // or they are unique, but then they should have been removed in the previous steps
    if (max_set.second.size() == 2) {
        return std::move(get_min_set_cover_2(std::move(sets), std::move(origin)));
    }

    // branching
    auto sets_branch = sets;

    // case where we do not take max_set to result
    sets.erase(max_set);
    auto res1 = std::move(min_set_cover(std::move(sets), origin));

    // case where we take to result max_set
    auto del_res = del(std::move(sets_branch), std::move(max_set.second), std::move(origin));
    auto res2 = std::move(min_set_cover(std::move(del_res.first), std::move(del_res.second)));
    res2.insert(std::move(max_set.first));

    return (res1.size() < res2.size()) ? (std::move(res1)) : (std::move(res2));
}

template <typename T>
std::pair<vset<T>, std::set<size_t>> get_msc_numbers(vector<vector<T>> sets, vector<T> set) {
    vset<T> new_sets(cmp<T>);

    size_t counter = 0;
    for (auto& v : sets) {
        new_sets.emplace(counter++, std::move(v));
    }

    std::set<T> origin(set.begin(), set.end());

    auto res1 = std::move(min_set_cover<T>(new_sets, std::move(origin)));
    return {std::move(new_sets), std::move(res1)};
}

template <typename T>
vector<vector<T>> get_msc(vector<vector<T>> sets, vector<T> set) {
    auto [vs, res1] = std::move(get_msc_numbers(sets, set));

    vector<vector<T>> res2;
    for (auto& element : vs) {
        if (res1.find(element.first) != res1.end()) {
            res2.push_back(std::move(element.second));
        }
    }

    return std::move(res2);
}

#endif