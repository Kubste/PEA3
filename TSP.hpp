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

using namespace std;

class TSP {

public:
    void set_matrix(vector<vector<int>> matrix);
    pair<vector<int>, int> NN();
    void explore_paths(vector<int> path, int path_length, vector<int> Q, int current_node, int start_node, pair<vector<int>, int> &resultsNN);
    pair<vector<int>, int> SA(float T0, int L0, int upper_bound, float a, int cooling_scheme, int solution_generator, int minutesSa);
    pair<vector<int>, int> TS(int max_iterations, int tenure, int restart_val, int upper_bound, int solution_generator, int minutesTS);

private:
    vector<vector<int>> matrix;
    pair<vector<int>, int> results;
    int min_value = INT_MAX;

    void set_min_value();
    pair<vector<int>, int> random();
    int calculate_path_length(vector<int> path);
    static double calculate_probability(float Xa, float Xk, float T);
    static bool decide_accept(double probability);
    static float calculate_T(float T, float a, int cooling_scheme, int k);
    pair<vector<int>, int> generate_solution(pair<vector<int>, int> path, int type);
    vector<pair<vector<int>, int>> generate_surroundings(vector<int> solution, int solution_generator);
    static bool is_in_tabu_list(const vector<pair<vector<int>, int>>& tabu_list, const vector<int>& solution);
    static void update_tabu_list(vector<pair<vector<int>, int>>& tabu_list);
};

#endif