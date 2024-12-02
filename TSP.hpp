#ifndef TSP_HPP
#define TSP_HPP
#include <vector>
#include <chrono>
#include <climits>
#include <iostream>
#include <algorithm>
#include <set>
#include <stack>
#include <queue>
#include <limits>
#include <utility>
#include <unordered_map>
#include <chrono>

using namespace std;

class TSP {

public:
    void set_matrix(vector<vector<int>> matrix);
    pair<vector<int>, int> NN();
    void explore_paths(vector<int> path, int path_length, vector<int> Q, int current_node, int start_node, pair<vector<int>, int> &resultsNN);

private:
    vector<vector<int>> matrix;
    pair<vector<int>, int> results;
    int min_value = INT_MAX;

    void set_min_value();

};

#endif