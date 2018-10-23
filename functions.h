//
// Created by chris on 10/22/2018.
//

#ifndef PROJECT2018_FUNCTIONS_H
#define PROJECT2018_FUNCTIONS_H

#include <vector>
#include <stdexcept>


class Functions {
public:
    static int dotProduct(std::vector<int> vector0, std::vector<int> vector1);

    static double dotProduct(std::vector<double> vector0, std::vector<double> vector1);

    static double dotProduct(std::vector<int> vector0, std::vector<double> vector1);

};

int Functions::dotProduct(std::vector<int> vector0, std::vector<int> vector1) {
    if (vector0.size() != vector1.size()) {
        throw new std::invalid_argument("dot product of different dimension arrays");
    }
    int d = vector0.size();

    int sum = 0.0;
    for (int i = 0; i < d; i++) {
        sum += vector0[i] * vector1[i];
    }

    return sum;
}

double Functions::dotProduct(std::vector<double> vector0, std::vector<double> vector1) {
    if (vector0.size() != vector1.size()) {
        throw new std::invalid_argument("dot product of different dimension arrays");
    }
    int d = vector0.size();

    double sum = 0.0;
    for (int i = 0; i < d; i++) {
        sum += vector0[i] * vector1[i];
    }

    return sum;
}

double Functions::dotProduct(std::vector<int> vector0, std::vector<double> vector1) {
    if (vector0.size() != vector1.size()) {
        throw new std::invalid_argument("dot product of different dimension arrays");
    }
    int d = vector0.size();

    double sum = 0.0;
    for (int i = 0; i < d; i++) {
        sum += vector0[i] * vector1[i];
    }

    return sum;
}


#endif //PROJECT2018_FUNCTIONS_H
