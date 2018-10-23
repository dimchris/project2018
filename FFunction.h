//
// Created by chris on 10/21/2018.
//

#ifndef PROJECT2018_FFUNCTION_H
#define PROJECT2018_FFUNCTION_H


#include "HashFunction.h"
#include "functions.h"
#include <random>
#include <assert.h>
#include <cmath>
#include <limits>


template<class T>

class FFunction : public HashFunction<T> {
public:
    virtual long getKey(std::vector<T> p);

    int h(std::vector<T> p, std::vector<double> v, double tau);

    int phi(std::vector<T> p);

    FFunction(int d, int k, int tableSize, int *r, int w);

    virtual ~FFunction();

private:

    int k;
    std::vector<double> **v;
    int *r;
    double *tau;
    int w;
    int tableSize;
    int maxPrime;

    void generateRandVector();
};

template<class T>
long FFunction<T>::getKey(std::vector<T> p) {
    return this->phi(p);
}

template<class T>
FFunction<T>::FFunction(int d, int k, int tableSize, int *r, int w):HashFunction<T>(d), k(k),
                                                                    tableSize(tableSize),
                                                                    w(w) {


    // initialize vector array
    v = new std::vector<double>*[k];

    // generate the random vectors
    generateRandVector();

    //cp the r table && set tau
    this->r = new int[k];
    this->tau = new double[k];
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.0, w);

    for (int i = 0; i < k; i++) {
        this->r[i] = r[i];
        this->tau[i] = distribution(generator);
    }


    // set max prime
    maxPrime = std::numeric_limits<int>::max() - 5;
}

template<class T>
FFunction<T>::~FFunction() {
    delete[] r;
    //delete each vector
    for (int i = 0; i < sizeof(v); i++) {
        delete v[i];
    }
    //delete v array
    delete[] v;

}

template<class T>
int FFunction<T>::h(std::vector<T> p, std::vector<double > v, double tau) {
    double result;
    assert(p.size() == HashFunction<T>::d);
    // calc h (check presentation)
    result = Functions::dotProduct(p, v) + tau;
    result /= w;

    return floor(result);
}

template<class T>
void FFunction<T>::generateRandVector() {
    // get the dimension from parent class
    int d = HashFunction<T>::d;

    // generate the k random vectors
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(1.0, 0.0);
    for (int i = 0; i < k; i++) {
        std::vector<double> *vector = new std::vector<double>();
        for (int j = 0; j < d; j++) {
            vector->push_back(distribution(generator));
        }
        v[i] = vector;
    }
}

template<class T>
int FFunction<T>::phi(std::vector<T> p) {
    assert(p.size() == HashFunction<T>::d);
    int result = 0;
    for (int i = 0; i < k; i++) {
        result += (r[i] * h(p, *(v[i]), tau[i])) % maxPrime;
    }
    result %= maxPrime;
    result %= tableSize;

    return result;
}


#endif //PROJECT2018_FFUNCTION_H
