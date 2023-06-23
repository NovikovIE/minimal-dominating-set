#include "gtest/gtest.h"

#include <ctime>
#include <random>
#include "min_dominating_set.h"

//////////////////////////////////////////////////////////
//////////////////  UNIT TESTS ///////////////////////////
//////////////////////////////////////////////////////////

TEST(matching, TEST1) {
    //  0 --- 1 --- 2 --- 3 --- 4 --- 5
    //                          |     |
    //                          8     6
    //                           \   /
    //                             7
    auto V = 9;
    auto E = 9;
    vector<vector<int>> t = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}, {7, 8}, {8, 4}};
    Graph graph(9);

    for (auto i = 0; i < E; ++i) {
        graph.add_edge(t[i][0], t[i][1]);
    }

    auto res = get_max_matching(graph);
    ASSERT_TRUE(res.size() == 4);
    vector<int> count(9, 0);
    for (auto& p : res) {
        count[p.first] += 1;
        count[p.second] += 1;
    }
    for (auto i : count) {
        ASSERT_TRUE(i <= 1);
    }
}

TEST(set_cover_2, TEST1) {
    vector<char> set = {'a', 'b', 'c', 'd'};
    std::map<char, int> m = {{'a', 0}, {'b', 1}, {'c', 2}, {'d', 3}};
    vset<char> sets(cmp<char>);
    vector<vector<char>> t = {{'a', 'c'}, {'b', 'd'}, {'a', 'd'}, {'c', 'd'}};
    for (size_t i = 0; i < t.size(); ++i) {
        sets.emplace(i, std::move(t[i]));
    }

    auto res = get_min_set_cover_2<char>(std::move(sets), set.size());
    ASSERT_TRUE(res.size() == 2);

    vector<bool> used(set.size(), false);
    for (auto& p : res) {
        for (auto& el : p) {
            used[m[el]] = true;
        }
    }
    for (auto i : used) {
        ASSERT_TRUE(i);
    }
}

TEST(set_cover, TEST1) {
    std::map<char, int> m = {{'a', 0}, {'b', 1}, {'c', 2}, {'d', 3}};

    vector<char> set = {'a', 'b', 'c', 'd'};
    vector<vector<char>> sets({{'a', 'c'}, {'b', 'd'}, {'a', 'd'}, {'c', 'd'}});
    vector<vector<char>> result = std::move(get_msc(sets, set));

    ASSERT_TRUE(result.size() == 2);

    vector<bool> used(set.size(), false);
    for (auto& p : result) {
        for (auto& el : p) {
            used[m[el]] = true;
        }
    }
    for (auto i : used) {
        ASSERT_TRUE(i);
    }
}

TEST(set_cover, TEST2) {
    vector<char> set = {'a', 'b', 'c', 'd'};
    std::map<char, int> m = {{'a', 0}, {'b', 1}, {'c', 2}, {'d', 3}};
    vector<vector<char>> sets = {{'a', 'b', 'c', 'd'}, {'a', 'c'}, {'b', 'd'}, {'a', 'd'}, {'c', 'd'}};

    vector<vector<char>> res = std::move(get_msc(sets, set));
    ASSERT_TRUE(res.size() == 1);

    vector<bool> used(set.size(), false);
    for (auto& p : res) {
        for (auto& el : p) {
            used[m[el]] = true;
        }
    }
    for (auto i : used) {
        ASSERT_TRUE(i);
    }
}

TEST(set_cover, TEST3) {
    vector<char> set = {'a', 'b', 'c', 'd', 'e', 'f'};
    std::map<char, int> m = {{'a', 0}, {'b', 1}, {'c', 2}, {'d', 3}, {'e', 4}, {'f', 5}};
    vector<vector<char>> t = {{'a', 'c'}, {'b', 'd'}, {'a', 'd', 'e'}, {'c', 'd', 'f'}, {'e', 'f'}};

    auto res = get_msc<char>(std::move(t), set);
    ASSERT_TRUE(res.size() == 3);

    vector<bool> used(set.size(), false);
    for (auto& p : res) {
        for (auto& el : p) {
            used[m[el]] = true;
        }
    }
    for (auto i : used) {
        ASSERT_TRUE(i);
    }
}

TEST(dominating_set, TEST1) {
    //    0 --- 1
    //   / \   / \
    //  2---3-4---5
    auto V = 6;
    auto E = 8;
    vector<vector<int>> t = {{0, 1}, {0, 2}, {0, 3}, {2, 3}, {3, 4}, {4, 5}, {1, 4}, {1, 5}};
    Graph graph(V);

    for (auto i = 0; i < E; ++i) {
        graph.add_edge(t[i][0], t[i][1]);
    }

    auto res = get_mds(graph);

    ASSERT_TRUE(res.size() == 2);

    vector<bool> used(V, false);

    for (auto& i : res) {
        used[i] = true;
        for (auto j : graph.edges[i]) {
            used[j] = true;
        }
    }
    for (auto i : used) {
        ASSERT_TRUE(i);
    }
}

TEST(dominating_set, TEST2) {
    //    0---1
    //   / \
    //  2---3---6
    //   \ /
    //    4---5
    auto V = 7;
    auto E = 8;
    vector<vector<int>> t = {{0, 1}, {0, 2}, {0, 3}, {2, 3}, {2, 4}, {3, 4}, {3, 6}, {4, 5}};
    Graph graph(V);

    for (auto i = 0; i < E; ++i) {
        graph.add_edge(t[i][0], t[i][1]);
    }

    auto res = get_mds(graph);

    ASSERT_TRUE(res.size() == 3);

    vector<bool> used(V, false);

    for (auto& i : res) {
        used[i] = true;
        for (auto j : graph.edges[i]) {
            used[j] = true;
        }
    }
    for (auto i : used) {
        ASSERT_TRUE(i);
    }
}

TEST(dominating_set, TEST3) {
    //    0---1
    //   / \
    //  2---3
    //   \
    //    4
    auto V = 5;
    auto E = 5;
    vector<vector<int>> t = {{0, 1}, {0, 2}, {0, 3}, {2, 3}, {2, 4}};
    Graph graph(V);

    for (auto i = 0; i < E; ++i) {
        graph.add_edge(t[i][0], t[i][1]);
    }

    auto res = get_mds(graph);

    ASSERT_TRUE(res.size() == 2);

    vector<bool> used(V, false);

    for (auto& i : res) {
        used[i] = true;
        for (auto j : graph.edges[i]) {
            used[j] = true;
        }
    }
    for (auto i : used) {
        ASSERT_TRUE(i);
    }
}

//////////////////////////////////////////////////////////
//////////////////  STRESS TESTS /////////////////////////
//////////////////////////////////////////////////////////

TEST(stress, TEST_CLIQUE) {
    std::cout << "TEST_CLIQUE\n";

    vector<vector<size_t>> metr;
    for (auto i = 5; i < 120; ++i) {
        Graph g(i);
        for (auto j = 0; j < i; ++j) {
            for (auto k = 0; k < i; ++k) {
                if (j == k) {
                    continue;
                }
                g.add_edge(j, k);
            }
        }
        auto res = std::move(get_mds(g));
        vector<bool> used(i, false);
        for (auto& t : res) {
            used[t] = true;
            for (auto j : g.edges[t]) {
                used[j] = true;
            }
        }
        for (auto j : used) {
            ASSERT_TRUE(j);
        }
        //        metrics.print();
        auto temp = metrics.get_metrics();
        temp.push_back(i);
        metr.push_back(std::move(temp));
    }

    for (auto& i : metr) {
        std::cout << i[0] << " ";
        std::cout << i[1] << " ";
        std::cout << i[2] << " ";
        std::cout << i[3] << " ";
        std::cout << i[4] << "\n";
    }
}

TEST(stress, TEST_HALF) {
    std::cout << "TEST_HALF\n";

    vector<vector<size_t>> metr;

    for (auto i = 5; i < 120; ++i) {
        Graph g(i);
        for (auto j = 0; j < i; ++j) {
            for (auto k = j + 1; k < i; ++k) {
                if (((j + k) % 2) == 0) {
                    continue;
                }
                g.add_edge(j, k);
            }
        }
        auto res = std::move(get_mds(g));
        vector<bool> used(i, false);
        for (auto& t : res) {
            used[t] = true;
            for (auto j : g.edges[t]) {
                used[j] = true;
            }
        }
        for (auto j : used) {
            ASSERT_TRUE(j);
        }

        auto temp = metrics.get_metrics();
        temp.push_back(i);
        metr.push_back(std::move(temp));
    }

    for (auto& i : metr) {
        std::cout << i[0] << " ";
        std::cout << i[1] << " ";
        std::cout << i[2] << " ";
        std::cout << i[3] << " ";
        std::cout << i[4] << "\n";
    }
}

TEST(stress, TEST_DISCONNECTED) {
    std::cout << "TEST_DISCONNECTED\n";

    vector<vector<size_t>> metr;

    for (auto i = 5; i < 120; ++i) {
        Graph g(i);
        auto res = std::move(get_mds(g));
        vector<bool> used(i, false);
        for (auto& t : res) {
            used[t] = true;
            for (auto j : g.edges[t]) {
                used[j] = true;
            }
        }
        for (auto j : used) {
            ASSERT_TRUE(j);
        }

        auto temp = metrics.get_metrics();
        temp.push_back(i);
        metr.push_back(std::move(temp));
    }

    for (auto& i : metr) {
        std::cout << i[0] << " ";
        std::cout << i[1] << " ";
        std::cout << i[2] << " ";
        std::cout << i[3] << " ";
        std::cout << i[4] << "\n";
    }
}

TEST(stress, TEST_ARTICULATION_POINT) {
    std::cout << "TEST_ARTICULATION_POINT\n";

    vector<vector<size_t>> metr;

    for (auto i = 5; i < 120; ++i) {
        Graph g(i);
        for (auto j = 0; j < i; ++j) {
            g.add_edge(j, 0);
            g.add_edge(0, j);
        }
        auto res = std::move(get_mds(g));
        vector<bool> used(i, false);
        for (auto& t : res) {
            used[t] = true;
            for (auto j : g.edges[t]) {
                used[j] = true;
            }
        }
        for (auto j : used) {
            ASSERT_TRUE(j);
        }

        auto temp = metrics.get_metrics();
        temp.push_back(i);
        metr.push_back(std::move(temp));
    }

    for (auto& i : metr) {
        std::cout << i[0] << " ";
        std::cout << i[1] << " ";
        std::cout << i[2] << " ";
        std::cout << i[3] << " ";
        std::cout << i[4] << "\n";
    }
}

TEST(stress, TEST_RANDOM) {
    std::cout << "TEST_RANDOM\n";

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist5(0, 5);

    vector<vector<size_t>> metr;

    for (auto i = 5; i < 45; ++i) {
        Graph g(i);
        for (auto j = 0; j < i; ++j) {
            for (auto k = j + 1; k < i; ++k) {
                if (dist5(rng) == 0) {
                    g.add_edge(j, k);
                    g.add_edge(k, j);
                }
            }
        }
        auto res = std::move(get_mds(g));
        vector<bool> used(i, false);
        for (auto& t : res) {
            used[t] = true;
            for (auto j : g.edges[t]) {
                used[j] = true;
            }
        }
        for (auto j : used) {
            ASSERT_TRUE(j);
        }

        auto temp = metrics.get_metrics();
        temp.push_back(i);
        metr.push_back(std::move(temp));
    }

    for (auto& i : metr) {
        std::cout << i[0] << " ";
        std::cout << i[1] << " ";
        std::cout << i[2] << " ";
        std::cout << i[3] << " ";
        std::cout << i[4] << "\n";
    }
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
