#include "TSP.hpp"

void TSP::set_matrix(vector<vector<int>> matrix) {
    this -> matrix = std::move(matrix);
    set_min_value();
}

void TSP::set_min_value() {
    for(auto & i : matrix) for(int j : i) if(j < min_value && j >= 0) min_value = j;
}

pair<vector<int>, int> TSP::SA(float T0, int L0, int upper_bound, float a, int cooling_scheme, int solution_generator) {
    float T = T0;
    int L = L0;
    pair<vector<int>, int> surrounding_solution;

    results.second = INT_MAX;
    if(upper_bound == 1) results = NN();
    else results = random();
    surrounding_solution = results;

    int k = 0;
    while(T > 1) {
        for(int i = 0; i < L; i++) {
            surrounding_solution = generate_solution(surrounding_solution, solution_generator);

            if(surrounding_solution.second < results.second) results = surrounding_solution;
            else {
                double probability = calculate_probability(results.second, surrounding_solution.second, T);
                if(decide_accept(probability)) results = surrounding_solution;
            }
        }
        T = calculate_T(T, a, cooling_scheme, k++);
    }
    return results;
}

pair<vector<int>, int> TSP::generate_solution(pair<vector<int>, int> solution, int type) {
    int last_element = solution.first.back();
    solution.first.pop_back();
    int index1 = rand() % solution.first.size();
    int index2 = rand() % solution.first.size();
    if(index1 > index2) {
        int temp = index1;
        index1 = index2;
        index2 = temp;
    }

    if(type == 1) swap(solution.first[index1], solution.first[index2]);
    else if(type == 2) reverse(solution.first.begin() + index1, solution.first.begin() + index2 + 1);
    else if(type == 3) {
        int value_to_insert = solution.first[index1];
        solution.first.erase(solution.first.begin() + index1);
        if(index2 > index1) --index2;
        solution.first.insert(solution.first.begin() + index2, value_to_insert);
    }

    if(solution.first.front() == last_element) solution.first.push_back(last_element);
    else solution.first.push_back(solution.first.front());
    solution.second = calculate_path_length(solution.first);
    return solution;
}

float TSP::calculate_T(float T, float a, int cooling_scheme, int k) {
    switch(cooling_scheme) {
        case 1: return T - a;
        case 2: return a * T;
        case 3: return T * pow(a, k);
        case 4: return T * pow((1 + pow(a, k)), -1);
        case 5: return T / (log(1 + k + 1));
    }
    return 0;
}

double TSP::calculate_probability(float Xa, float Xk, float T) {
    const double EulerConstant = std::exp(1.0);
    return pow(EulerConstant, (Xa - Xk) / T);
}

bool TSP::decide_accept(double probability) {
    double X = ((double) rand() / (double) RAND_MAX);
    if(X < probability) return true;
    else return false;
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