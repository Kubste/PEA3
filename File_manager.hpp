#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP
#include <string>
#include <vector>
#include <chrono>
#include <cmath>
#define M_PI 3.14159265358979323846

using namespace std;

class File_manager {

public:
    tuple<vector<string>, vector<int>, vector<float>> read_config_file(const string& path);
    pair<vector<vector<int>>, int> read_data_file(const string& path);
    void write_to_file(const string& data_name, const string& results_name, int optimal_value, vector<chrono::duration<double,
            micro>> total_times, chrono::duration<double, micro> time, float absolute_error, float relative_error);

private:
    vector<vector<int>> set_matrix(vector<vector<double>> buffer);
    static double haversine(double lat1, double lon1, double lat2, double lon2);
};

#endif