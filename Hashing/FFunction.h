//
// Created by chris on 10/21/2018.
//

#ifndef PROJECT2018_FFUNCTION_H
#define PROJECT2018_FFUNCTION_H


#include "HashFunction.h"
#include "../UtilClasses/UsefullFunctions.h"
#include "../UtilClasses/MyGenerator.h"
#include <random>
#include <assert.h>
#include <cmath>
#include <limits>
#include <iostream>
#include <chrono>


template<class T>

class FFunction : public HashFunction<T> {
public:
    virtual long getKey(std::vector<T> *p);

    int h(std::vector<T> p, std::vector<double> v, double tau);

    virtual int phi(std::vector<T> *p);

    FFunction(int d, int k, int tableSize, int *r, int w);

    virtual ~FFunction();

private:

    int *r;
    int w;
    int tableSize;
    int maxPrime;

    void generateRandVector();

protected:
    double *tau;

    int k;

    std::vector<double> **v;
};

template<class T>
long FFunction<T>::getKey(std::vector<T> *p) {
    if (p->size() != HashFunction<T>::d) {
        throw std::invalid_argument("Incompatible dimensions: hashFunction, vector");
    }
    return this->phi(p);
}

template<class T>
FFunction<T>::FFunction(int d, int k, int tableSize, int *r, int w):HashFunction<T>(d), k(k),
                                                                    tableSize(tableSize),
                                                                    w(w) {


    // initialize vector array
    v = new std::vector<double> *[k];

    // generate the random vectors
    generateRandVector();

    //cp the r table && set tau
    this->r = new int[k];
    this->tau = new double[k];
    MyGenerator *myGenerator = MyGenerator::getInstance();

    std::uniform_real_distribution<double> distribution(0.0, w);
    std::default_random_engine *generator = myGenerator->getGenerator();
    if (r != NULL)
        for (int i = 0; i < k; i++) {
            this->r[i] = r[i];
            this->tau[i] = distribution(*generator);
        }


    // set max prime
    maxPrime = std::numeric_limits<int>::max() - 5;
}

template<class T>
FFunction<T>::~FFunction() {
    if (r != NULL)
        delete[] r;
    //delete each vector
    for (int i = 0; i < sizeof(v); i++) {
        delete v[i];
    }
    //delete v array
    delete[] v;

}

template<class T>
int FFunction<T>::h(std::vector<T> p, std::vector<double> v, double tau) {
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
    MyGenerator *myGenerator = MyGenerator::getInstance();
    std::default_random_engine *generator = myGenerator->getGenerator();
    std::normal_distribution<double> distribution(0.0, 1.0);
    for (int i = 0; i < k; i++) {
        std::vector<double> *vector = new std::vector<double>();
        for (int j = 0; j < d; j++) {
            vector->push_back(distribution(*generator));
        }
        v[i] = vector;
    }
}

template<class T>
int FFunction<T>::phi(std::vector<T> *p) {
    assert(p->size() == HashFunction<T>::d);
    long long result = 0;
    for (int i = 0; i < k; i++) {
        result += r[i] * h(*p, *(v[i]), tau[i]);
    }
    result = Functions::lmod(result, maxPrime);
    result = Functions::lmod(result, tableSize);

    return result;
}


#endif //PROJECT2018_FFUNCTION_H
