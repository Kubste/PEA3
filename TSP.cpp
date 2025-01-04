#include "TSP.hpp"

void TSP::set_matrix(vector<vector<int>> matrix) {
    this -> matrix = std::move(matrix);
    set_min_value();
}

void TSP::set_min_value() {
    for(auto & i : matrix) for(int j : i) if(j < min_value && j >= 0) min_value = j;
}

pair<vector<int>, int> TSP::TS(int restart_val, int upper_bound, int solution_generator, int minutes, int optimal_value, float tenure_factor, float list_factor) {
    chrono::time_point<chrono::steady_clock> start = chrono::steady_clock::now();
    int restart = restart_val;
    results.second = INT_MAX;
    pair<vector<int>, int> x0;
    if(upper_bound == 1) x0 = NN();
    else x0 = random();
    results = x0;
    int current_path_length = results.second;
    pair<vector<int>, int> xa = x0;
    vector<pair<vector<int>, int>> tabu_list;

    int tenure = tenure_factor * float(matrix.size()) * float((matrix.size() - 1));
    if(solution_generator != 3) tenure = tenure / 2;

    while(chrono::duration_cast<chrono::minutes>(chrono::steady_clock::now() - start).count() < minutes) {
        if(results.second == optimal_value) return results;
        vector<pair<vector<int>, int>> surroundings = generate_surroundings(xa.first, solution_generator);
        pair<vector<int>, int> best_solution;
        best_solution.second = INT_MAX;

        for(const auto& solution : surroundings) {
            if(solution.second < best_solution.second && (!is_in_tabu_list(tabu_list, solution.first) || solution.second < results.second)) {
                best_solution = solution;
                best_solution.second = solution.second;
                tabu_list.emplace_back(solution.first, tenure);
            }
        }

        xa = best_solution;
        if(best_solution.second < results.second) results = best_solution;

        update_tabu_list(tabu_list);

        if(current_path_length == results.second) restart--;
        else {
            current_path_length = results.second;
            restart = restart_val;
        }
        if(restart < 0) {
            restart = restart_val;
            if(upper_bound) xa = NN();
            else xa = random();
            tabu_list.clear();
        }
    }
    return results;
}

void TSP::update_tabu_list(vector<pair<vector<int>, int>>& tabu_list) {
    for(auto& element : tabu_list) element.second--;
    tabu_list.erase(remove_if(tabu_list.begin(), tabu_list.end(), [](const pair<vector<int>, int>& tabu) { return tabu.second <= 0; }), tabu_list.end());
}

bool TSP::is_in_tabu_list(const vector<pair<vector<int>, int>>& tabu_list, const vector<int>& solution) {
    for(auto& tabu : tabu_list) if(tabu.first == solution) return true;
    return false;
}

vector<pair<vector<int>, int>> TSP::generate_surroundings(vector<int> solution, int solution_generator) {
    vector<pair<vector<int>, int>> surroundings;
    solution.pop_back();
    int size = solution.size();

    for(int i = 0; i < size; i++) {
        for(int j = i + 1; j < size; j++) {
            vector<int> new_solution = solution;
            if(solution_generator == 1) swap(new_solution[i], new_solution[j]);
            else if(solution_generator == 2) reverse(new_solution.begin() + i, new_solution.begin() + j + 1);
            else if(solution_generator == 3) {
                int value_to_insert = new_solution[i];
                new_solution.erase(new_solution.begin() + i);
                new_solution.insert(new_solution.begin() + j, value_to_insert);
            }
            new_solution.push_back(new_solution.front());
            int new_solution_length = calculate_path_length(new_solution);
            if(new_solution_length != INT_MAX) surroundings.emplace_back(new_solution, new_solution_length);
        }
    }
    return surroundings;
}

pair<vector<int>, int> TSP::random() {
    pair<vector<int>, int> randomResults;
    vector<int> path;
    random_device random;
    mt19937 g(static_cast<unsigned>(chrono::system_clock::now().time_since_epoch().count()));
    randomResults.second = INT_MAX;

    for(int i = 0; i < matrix.size(); i++) path.push_back(i);

    while(randomResults.second == INT_MAX) {
        shuffle(path.begin(), path.end(), g);
        reverse(path.begin() + g() % matrix.size(), path.end());

        path.push_back(path.front());
        randomResults.first = path;
        randomResults.second = calculate_path_length(path);

        path.pop_back();
    }

    return randomResults;
}

int TSP::calculate_path_length(vector<int> path) {
    int path_length = 0;

    for(int i = 0; i < path.size() - 1; i++) {
        if(matrix[path[i]][path[i + 1]] == -1) return INT_MAX;
        else path_length = path_length + matrix[path[i]][path[i + 1]];
    }
    return path_length;
}

pair<vector<int>, int> TSP::NN() {
    pair<vector<int>, int> resultsNN;
    resultsNN.second = INT_MAX;
    vector<int> path;
    vector<int> Q;

    for(int j = 0; j < matrix.size(); j++) {

        path.push_back(j);

        for(int i = 0; i < matrix.size(); i++) if(i != j) Q.push_back(i);
        explore_paths(path, 0, Q, j, j, resultsNN);

        path.clear();
        Q.clear();
    }
    return resultsNN;
}

void TSP::explore_paths(vector<int> path, int path_length, vector<int> Q, int current_node, int start_node,
                        pair<vector<int>, int> &resultsNN) {
    vector<pair<int, int>> min_edges;
    int min_edge_value = INT_MAX;
    vector<int> new_path;
    vector<int> new_Q;
    int next_node;
    int edge_length;

    if(Q.empty()) {
        if(matrix[current_node][start_node] != -1) {
            path_length += matrix[current_node][start_node];
            path.push_back(start_node);

            if(path_length < resultsNN.second) {
                resultsNN.first = path;
                resultsNN.second = path_length;
            }
        }
        return;
    }

    for(int i = 0; i < matrix.size(); i++) {
        if(matrix[current_node][i] != -1 && find(Q.begin(), Q.end(), i) != Q.end()) {
            if(matrix[current_node][i] < min_edge_value) {
                min_edges.clear();
                min_edge_value = matrix[current_node][i];
                min_edges.emplace_back(i, matrix[current_node][i]);
            } else if(matrix[current_node][i] == min_edge_value) min_edges.emplace_back(i, matrix[current_node][i]);
        }
    }

    for(auto & min_edge : min_edges) {
        next_node = min_edge.first;
        edge_length = min_edge.second;

        new_path = path;
        new_path.push_back(next_node);
        new_Q = Q;
        new_Q.erase(remove(new_Q.begin(), new_Q.end(), next_node), new_Q.end());

        explore_paths(new_path, path_length + edge_length, new_Q, next_node, start_node, resultsNN);
    }
}