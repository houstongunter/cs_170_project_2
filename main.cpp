#include <fstream>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <cmath>
#include <limits>
#include <chrono>

using namespace std;
using namespace std::chrono;

vector<vector<double>> data;

// https://www.w3schools.com/cpp/cpp_files.asp
void data_processing(const string file_name) {
    ifstream filein(file_name);
    string line;

    while (getline(filein, line)) {
        vector<double> row;
        double num = 0.0;
        istringstream stream(line);
        while (stream >> num) {
            row.push_back(num);
        }
        data.push_back(row);
    }
}

double calculate_distance(vector<double> vector1, vector<double> vector2) {
    double num = 0;
    for (int i = 0; i < vector1.size(); i++) {
        num += pow(vector2[i] - vector1[i], 2);
    }
    num = sqrt(num);
    return num;
}

double leave_one_out_cross_validation_forward(vector<int> current_features, int feature_to_add) {
    int num_correctly_classified = 0;
    vector<int> features_to_use = current_features;
    features_to_use.push_back(feature_to_add);

    for (int i = 0; i < data.size(); i++) {
        vector<double> object_to_classify;
        for (int j = 0; j < features_to_use.size(); j++) {
            object_to_classify.push_back(data[i][features_to_use[j]]);
        }
        double label_object_to_classify = data[i][0];
        double nearest_neighbor_distance = numeric_limits<double>::infinity();
        int nearest_neighbor_location = numeric_limits<int>::infinity();
        double nearest_neighbor_label = -1;
        for (int j = 0; j < data.size(); j++) {
            if (j != i) {
                vector<double> neighbor;
                for (int k = 0; k < features_to_use.size(); k++) {
                    neighbor.push_back(data[j][features_to_use[k]]);
                }
                double distance = calculate_distance(object_to_classify, neighbor);
                if (distance < nearest_neighbor_distance) {
                    nearest_neighbor_distance = distance;
                    nearest_neighbor_location = j;
                    nearest_neighbor_label = data[nearest_neighbor_location][0];
                }
            }
        }
        if (label_object_to_classify == nearest_neighbor_label) {
            num_correctly_classified += 1;
        }
    }

    double accuracy = (double)num_correctly_classified / (double)data.size();
    cout << "Using feature(s) ";
    for (int j = 0; j < features_to_use.size(); j++) {
            cout << features_to_use[j];
            cout << ", ";
    } 
    cout << "accuracy is " << accuracy << endl;
    return accuracy;
}

void forward_selection() {
    vector<int> current_set_of_features;
    vector<vector<int>> all_feature_sets; 
    vector<double> best_accuracies;
    vector<int> best_feature_set;
    double best_overall_accuracy = 0;

     for (int i = 0; i < data[0].size() - 1; i++) {
        int feature_to_add_at_this_level;
        double best_so_far_accuracy = 0;
        for (int j = 1; j < data[0].size(); j++) {
            int numOccur = count(current_set_of_features.begin(), current_set_of_features.end(), j); // https://www.geeksforgeeks.org/cpp/check-if-vector-contains-given-element-in-cpp/
            if (numOccur == 0) {
                cout << "--Considering adding the " << j << " feature" << endl;
                double accuracy = leave_one_out_cross_validation_forward(current_set_of_features, j);
                if (accuracy > best_so_far_accuracy) {
                    best_so_far_accuracy = accuracy;
                    feature_to_add_at_this_level = j;
                }
            }
        }
        current_set_of_features.push_back(feature_to_add_at_this_level);
        all_feature_sets.push_back(current_set_of_features);
        best_accuracies.push_back(best_so_far_accuracy);

        if (best_so_far_accuracy > best_overall_accuracy) {
            best_overall_accuracy = best_so_far_accuracy;
            best_feature_set = current_set_of_features;
        }

         if (best_accuracies.size() > 1) {
            if (best_so_far_accuracy < best_accuracies[i-1]) {
                cout << "(Warning, Accuracy has decreased. Continuing search incase of local maxima)" << endl;
            }
        }
        cout << "Feature set ";
        for (int j = 0; j < current_set_of_features.size(); j++) {
            cout << current_set_of_features[j];
            cout << ", ";
        } 
        cout << "was best, accuracy is " << best_so_far_accuracy << "\n" << endl;
    }
    cout << "Finished search" << endl;
    cout << "Highest accuracy is " << best_overall_accuracy << " using features ";
    for (int j = 0; j < best_feature_set.size(); j++) {
            cout << best_feature_set[j];
            cout << ", ";
        } 
    cout << endl;
    cout << "\nAccuracy at each level:" << endl;
    for (int i = 0; i < all_feature_sets.size(); i++) {
        cout << "Level " << i + 1 << ": features ";
        for (int j = 0; j < all_feature_sets[i].size(); j++) {
            cout << all_feature_sets[i][j] << ", ";
        }
        cout << "accuracy: " << best_accuracies[i] << endl;
    }
}

double leave_one_out_cross_validation_backward(vector<int> current_features, int feature_to_remove = -1) {
    int num_correctly_classified = 0;
    vector<int> features_to_use = current_features;

    //https://www.geeksforgeeks.org/cpp/how-to-remove-an-element-from-vector-in-cpp/
    if (feature_to_remove != -1) {
        features_to_use.erase(find(features_to_use.begin(), features_to_use.end(), feature_to_remove));
    }

    for (int i = 0; i < data.size(); i++) {
        vector<double> object_to_classify;
        for (int j = 0; j < features_to_use.size(); j++) {
            object_to_classify.push_back(data[i][features_to_use[j]]);
        }
        double label_object_to_classify = data[i][0];
        double nearest_neighbor_distance = numeric_limits<double>::infinity();
        int nearest_neighbor_location = numeric_limits<int>::infinity();
        double nearest_neighbor_label;
        for (int j = 0; j < data.size(); j++) {
            if (j != i) {
                vector<double> neighbor;
                for (int k = 0; k < features_to_use.size(); k++) {
                    neighbor.push_back(data[j][features_to_use[k]]);
                }
                double distance = calculate_distance(object_to_classify, neighbor);
                if (distance < nearest_neighbor_distance) {
                    nearest_neighbor_distance = distance;
                    nearest_neighbor_location = j;
                    nearest_neighbor_label = data[nearest_neighbor_location][0];
                }
            }
        }
        if (label_object_to_classify == nearest_neighbor_label) {
            num_correctly_classified += 1;
        }
    }

    double accuracy = (double)num_correctly_classified / (double)data.size();
    cout << "Using feature(s) ";
    for (int j = 0; j < features_to_use.size(); j++) {
            cout << features_to_use[j];
            cout << ", ";
    } 
    cout << "accuracy is " << accuracy << endl;
    return accuracy;
}

void backward_elimination() {
    vector<int> current_set_of_features;

    for (int i = 1; i < data[0].size(); i++) {
        current_set_of_features.push_back(i);
    }

    double accuracy = leave_one_out_cross_validation_backward(current_set_of_features);
    cout << endl;

    vector<vector<int>> all_feature_sets; 
    vector<double> best_accuracies;
    vector<int> best_feature_set;
    double best_overall_accuracy = 0;
    all_feature_sets.push_back(current_set_of_features);
    best_accuracies.push_back(accuracy);

     for (int i = 0; i < data[0].size() - 2; i++) {
        int feature_to_remove_at_this_level;
        double best_so_far_accuracy = 0;
        for (int j = 1; j < data[0].size(); j++) {
            int numOccur = count(current_set_of_features.begin(), current_set_of_features.end(), j); // https://www.geeksforgeeks.org/cpp/check-if-vector-contains-given-element-in-cpp/
            if (numOccur > 0) {
                cout << "--Considering removing the " << j << " feature" << endl;
                accuracy = leave_one_out_cross_validation_backward(current_set_of_features, j);
                if (accuracy > best_so_far_accuracy) {
                    best_so_far_accuracy = accuracy;
                    feature_to_remove_at_this_level = j;
                }
            }
        }
        current_set_of_features.erase(find(current_set_of_features.begin(), current_set_of_features.end(), feature_to_remove_at_this_level));
        all_feature_sets.push_back(current_set_of_features);
        best_accuracies.push_back(best_so_far_accuracy);

        if (best_so_far_accuracy > best_overall_accuracy) {
            best_overall_accuracy = best_so_far_accuracy;
            best_feature_set = current_set_of_features;
        }

         if (best_accuracies.size() > 1) {
            if (best_so_far_accuracy < best_accuracies[i-1]) {
                cout << "(Warning, Accuracy has decreased. Continuing search incase of local maxima)" << endl;
            }
        }
        cout << "Feature set ";
        for (int j = 0; j < current_set_of_features.size(); j++) {
            cout << current_set_of_features[j];
            cout << ", ";
        } 
        cout << "was best, accuracy is " << best_so_far_accuracy << "\n" << endl;
    }
    cout << "Finished search" << endl;
    cout << "Highest accuracy is " << best_overall_accuracy << " using features ";
    for (int j = 0; j < best_feature_set.size(); j++) {
            cout << best_feature_set[j];
            cout << ", ";
        } 
    cout << endl;
    cout << "\nAccuracy at each level:" << endl;
    for (int i = 0; i < all_feature_sets.size(); i++) {
        cout << "Level " << i + 1 << ": features ";
        for (int j = 0; j < all_feature_sets[i].size(); j++) {
            cout << all_feature_sets[i][j] << ", ";
        }
        cout << "accuracy: " << best_accuracies[i] << endl;
    }
}

double default_rate() {
    int num_1s = 0;
    int num_2s = 0;
    int total = 0;
    double defaultRate;
    for (int i = 0; i < data.size(); i++) {
        if (data[i][0] == 1) {
            num_1s += 1;
        } else {
            num_2s += 1;
        }
        total += 1;
    }
    if (num_1s >= num_2s) {
        defaultRate = (double)num_1s / (double)total;
    } else {
        defaultRate = (double)num_2s / (double)total;
    }
    return defaultRate;
}

    
int main() {
    string file_name;
    cout << "Type in the name of file to test: ";
    cin >> file_name;
    data_processing(file_name);

    cout << "Type the number of the algorithm you want to run" << endl;
    cout << "1) Forward Selection" << endl;
    cout << "2) Backward Elimination" << endl;

    string user_input;
    cin >> user_input;
    while (user_input != "1" && user_input != "2") {
        cout << "Unrecognized input, try again" << endl;
        cin >> user_input;
    }

    // https://www.geeksforgeeks.org/cpp/measure-execution-time-function-cpp/
    auto start = high_resolution_clock::now();

    double defaultRate = default_rate();
    cout << "Default rate is " << defaultRate << endl;

    if (user_input == "1") {
        forward_selection();
    } else {
        backward_elimination();
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    cout << "Time taken by function: "
         << duration.count() << " seconds" << endl;

    return 0;
}