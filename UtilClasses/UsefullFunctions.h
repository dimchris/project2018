//
// Created by chris on 10/22/2018.
//

#ifndef PROJECT2018_FUNCTIONS_H
#define PROJECT2018_FUNCTIONS_H

#include <vector>
#include <stdexcept>


class Functions {
public:
    inline static int dotProduct(std::vector<int> vector0, std::vector<int> vector1);

    inline static double dotProduct(std::vector<double> vector0, std::vector<double> vector1);

    inline static double dotProduct(std::vector<int> vector0, std::vector<double> vector1);


    inline static int mod(int a, int b);

    inline static long long int lmod(long long a, long long b);

    inline static bool isPowerOfTwo(unsigned int x);

    inline static bool differAtOneBitPos(unsigned int a, unsigned int b);


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

int Functions::mod(int a, int b) {
    return (b + (a % b)) % b;
}

long long int Functions::lmod(long long a, long long b) {
    return (b + (a % b)) % b;
}

// function to check if x is power of 2
bool Functions::isPowerOfTwo(unsigned int x) {
    // First x in the below expression is
    // for the case when x is 0
    return x && (!(x & (x - 1)));
}

// function to check whether the two numbers
// differ at one bit position only
bool Functions::differAtOneBitPos(unsigned int a, unsigned int b) {
    return isPowerOfTwo(a ^ b);
}

#endif //PROJECT2018_FUNCTIONS_H
