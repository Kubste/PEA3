#include "Main.hpp"
#include <iostream>

using namespace std;

void Main::run() {
    pair<vector<int>, int> results;
    vector<chrono::duration<double, micro>> times;
    pair<vector<vector<int>>, int> data;
    chrono::duration<double, micro> time{};
    chrono::high_resolution_clock::time_point t0;

    tuple<vector<string>, vector<int>, vector<float>> config_data = file_manager.read_config_file(config_path);
    assign_parameters(get<0>(config_data), get<1>(config_data), get<2>(config_data));
    data = file_manager.read_data_file(data_path);
    matrix = data.first;
    optimal_value = data.second;

    if(progress_indicator == 0) print_info();

    tsp.set_matrix(matrix);

    cout << "Zakonczono przygotowywanie" << endl;
    system("pause");

    for(int i = 0; i < repetitions; i++) {
        t0 = chrono::high_resolution_clock::now();
        if(method == 1) results = tsp.SA(T0, L0, upper_bound, a, cooling_scheme, solution_generator);

        time = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - t0);
        print_partial_results(results, i + 1, time);
    }

    if(progress_indicator) print_info();
    print_total_results();
    file_manager.write_to_file(data_path.substr(data_path.find_last_of('/') + 1), result_path, optimal_value, total_times,
                               total_time/time_measurements, total_absolute_error/repetitions, total_relative_error/repetitions);
}

void Main::assign_parameters(vector<string> parameters_string, vector<int> parameters_int, vector<float> parameters_float) {
    data_path = parameters_string[0];
    result_path = parameters_string[1];
    method = parameters_int[0];
    if(parameters_int[1] == -1) minutesSA = INT_MAX;
    else minutesSA = parameters_int[1];
    if(parameters_int[2] == -1) minutesTS = INT_MAX;
    else minutesTS = parameters_int[2];
    cooling_scheme = parameters_int[3];
    solution_generator = parameters_int[4];
    L0 = parameters_int[5];
    upper_bound = parameters_int[6];
    repetitions = parameters_int[7];
    progress_indicator = parameters_int[8];
    T0 = parameters_float[0];
    a = parameters_float[1];
}

void Main::print_info() {

    size_t position = data_path.find_last_of('/');
    cout << endl << "Plik zawierajacy dane problemu: " << data_path.substr(position + 1) << endl;
    cout << "Wynik optymalny: " << optimal_value << endl;
    cout << "Wybrana metoda: ";
    if(method == 1) cout << "Symulowane wyzarzanie" << endl;
    else if(method == 2) cout << "Tabu Search" << endl;
    cout << "Liczba powtorzen przeszukania: " << repetitions << endl;
    if(method == 1 && minutesSA != INT_MAX) cout << "Maksymalny czas przeszukania: " << minutesSA << " min" << endl;
    else if(method == 1) cout << "Brak ograniczenia czasowego" << endl;
    else if(method == 2 && minutesTS != INT_MAX) cout << "Maksymalny czas przeszukania: " << minutesTS << " min" << endl;
    else if(method == 2) cout << "Brak ograniczenia czasowego" << endl;
    cout << endl;
}

void Main::print_partial_results(pair<vector<int>, int> results, int repetition, chrono::duration<double, micro> time) {
    float absolute_error;
    float relative_error;

    cout << "Wykonano " << repetition << " przeszukanie" << endl;
    cout << "Otrzymana najkrotsza sciezka: ";
    for(int i = 0; i < results.first.size() - 1; i++) cout << results.first[i] << " -> ";
    cout << results.first.back() << endl;
    cout << "Dlugosc otrzymanej sciezki: " << results.second << endl;

    cout << "Czas rozwiazania: " << repetition << ": ";
    cout << fixed << setprecision(3);
    if(time.count() >= 180000000) cout << chrono::duration<double, ratio<60>>(time).count() << " min" << endl;
    else if(time.count() >= 1000000) cout << chrono::duration<double>(time).count() << " s" << endl;
    else if(time.count() >= 1000) cout << chrono::duration<double, milli>(time).count() << " ms" << endl;
    else cout << time.count() << " micro" << endl;
    cout.unsetf(ios::fixed);
    total_time = total_time + time;
    total_times.emplace_back(time);
    if(time.count() != 0) time_measurements++;

    absolute_error = results.second - optimal_value;
    relative_error = (absolute_error / optimal_value);
    total_absolute_error = total_absolute_error + absolute_error;
    total_relative_error = total_relative_error + relative_error;

    cout << "Blad bezwzgledny dla rozwiazania " << repetition << ": " << absolute_error << endl;
    cout << "Blad wzgledny dla rozwiazania " << repetition << ": " << relative_error << " = " << relative_error * 100 << "% " << endl << endl;

    cout << endl;
}

void Main::print_total_results() {

    cout << endl << "Wykonano " << repetitions << " powtorzen" << endl;
    cout << fixed << setprecision(3);
    cout << "Sredni czas wyznaczenia rozwiazania: ";
    if(total_time.count() / time_measurements >= 180000000) cout << chrono::duration<double, ratio<60>>(total_time).count() << " min" << endl;
    else if(total_time.count() / time_measurements >= 1000000) cout << chrono::duration<double>(total_time).count() / time_measurements << " s" << endl;
    else if(total_time.count() / time_measurements >= 1000) cout << chrono::duration<double, milli>(total_time).count() / time_measurements << " ms" << endl;
    else cout << total_time.count() / time_measurements << " micro" << endl;
    cout.unsetf(ios::fixed);
    cout << "Sredni blad bezwzgledny: " << total_absolute_error / repetitions << endl;
    cout << "Sredni blad wzgledny: " << total_relative_error / repetitions << " = " << (total_relative_error / repetitions) * 100 << "% " << endl;

}

int Main::calculate_path_length(vector<int> path) {
    int path_length = 0;

    for(int i = 0; i < path.size() - 1; i++) {
        if(matrix[path[i]][path[i + 1]] == -1) return -1;
        else path_length = path_length + matrix[path[i]][path[i + 1]];
    }
    return path_length;
}

int main() {
    srand(time(nullptr));
    Main main_obj{};
    main_obj.run();
    cout << endl;
    system("pause");

    return 0;
}