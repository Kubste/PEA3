#include "File_manager.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <tuple>

using namespace std;

tuple<vector<string>, vector<int>, vector<float>> File_manager::read_config_file(const string& path) {
    tuple<vector<string>, vector<int>, vector<float>> results;
    string line;
    ifstream file;
    string sub_str;
    int line_num = 0;

    file.open(path);

    if(file.is_open()) {
        while(getline(file, line)) {
            size_t position = line.find('#');
            sub_str = line.substr(position + 2);
            if(line_num < 2) get<0>(results).push_back("files/" + sub_str);
            else if(line_num == 5 || line_num == 6) get<2>(results).push_back(stof(sub_str));
            else get<1>(results).push_back(stoi(sub_str));

            line_num++;
        }
    }

    file.close();

    return results;
}

pair<vector<vector<int>>, int> File_manager::read_data_file(const string& path) {
    pair<vector<vector<int>>, int> data;
    vector<vector<int>> matrix;
    vector<vector<double>> buffer;
    string line;
    ifstream file;

    file.open(path);

    if(file.is_open()) {
        getline(file, line);
        if(stoi(line) == 0) {
            getline(file, line);
            int size = stoi(line);
            for(int i = 0; i < size; i++) {
                getline(file, line);
                stringstream ss(line);
                vector<int> row;
                int number;
                while(ss >> number) row.push_back(number);
                matrix.push_back(row);
            }
        } else if(stoi(line) == 1) {
            getline(file, line);
            int size = stoi(line);
            for(int i = 0; i < size; i++) {
                getline(file, line);
                stringstream ss(line);
                vector<double> row;
                float number;
                while(ss >> number) row.push_back(number);
                buffer.push_back(row);
            }
            matrix = set_matrix(buffer);
        }
        getline(file, line);
        data.second = stoi(line);
    }

    file.close();

    data.first = matrix;
    return data;
}

vector<vector<int>> File_manager::set_matrix(vector<vector<double>> buffer) {
    vector<vector<int>> matrix;
    vector<int> row;

    for(int i = 0; i < buffer.size(); i++) {
        for(int j = 0; j < buffer.size(); j++) {
            if(i == j) row.push_back(-1);
            else row.push_back(static_cast<int>(round((haversine(buffer[i][0], buffer[i][1], buffer[j][0], buffer[j][1])))));
        }
        matrix.push_back(row);
        row.clear();
    }
    return matrix;
}

double File_manager::haversine(double lat1, double lon1, double lat2, double lon2) {
    const double rad = 6371;
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;

    lat1 = (lat1) * M_PI / 180.0;
    lat2 = (lat2) * M_PI / 180.0;

    double sq = pow(sin(dLat / 2), 2) + pow(sin(dLon / 2), 2) * cos(lat1) * cos(lat2);
    return 2 * rad * asin(sqrt(sq));
}

void File_manager::write_to_file(const string& data_name, const std::string& results_name, int optimal_value, vector<chrono::duration<double, micro>> total_times,
                                 chrono::duration<double, micro> time, float absolute_error, float relative_error) {

    ofstream file(results_name, ios::trunc);

    file << "Nazwa instancji," << data_name << "\n";
    file << "Wynik optymalny," << optimal_value << "\n";
    file << "Sredni czas wykonania," << time.count() << "\n";
    file << "Sredni blad bezwzgledny," << absolute_error << "\n";
    file << "Sredni blad wzgledny," << relative_error << "\n";
    file << "Sredni blad wzgledny(w procentach)," << relative_error * 100 << "%" "\n";
    file << "Numer proby,Czas wykonania" << "\n";

    for(int i = 0; i < total_times.size(); i++) {
        file << i + 1 << "," << total_times[i].count() << "\n";
    }

    file.close();
}