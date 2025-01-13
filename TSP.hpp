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
#include <random>
#include <map>
#include <deque>

using namespace std;

class TSP {

public:
    void set_matrix(vector<vector<int>> matrix);
    pair<vector<int>, int> NN();
    void explore_paths(vector<int> path, int path_length, vector<int> Q, int current_node, int start_node, pair<vector<int>, int> &resultsNN);
    pair<vector<int>, int> TS(float end_factor, float restart_factor, int upper_bound, int solution_generator, int minutes, int optimal_value, float tenure_factor, float list_factor);

private:
    vector<vector<int>> matrix;
    pair<vector<int>, int> results;
    int min_value = INT_MAX;

    void set_min_value();
    pair<vector<int>, int> random();
    int calculate_path_length(vector<int> path);
    vector<pair<vector<int>, int>> generate_surroundings(vector<int> solution, int solution_generator);
    static bool is_in_tabu_list(const deque<pair<vector<int>, int>>& tabu_list, const vector<int>& solution);
    static void update_tabu_list(deque<pair<vector<int>, int>>& tabu_list);
};

#endif