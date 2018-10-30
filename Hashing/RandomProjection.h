//
// Created by chris on 10/28/2018.
//

#ifndef PROJECT2018_RANDOPROJECTION_H
#define PROJECT2018_RANDOPROJECTION_H


#include <random>
#include "HashFunction.h"
#include "../UtilClasses/MyGenerator.h"
#include "../UtilClasses/UsefullFunctions.h"

template<class T>
class RandomProjection : public HashFunction<T> {
private:
    int k;
    std::vector<double> **r;

    void generateRandVector();

    int h(std::vector<T> *p, std::vector<double> *r);

    long G(std::vector<T> *p);


public:

    RandomProjection(int d, int k);

    long getKey(std::vector<T> *p) override;

};

template<class T>
long RandomProjection<T>::getKey(std::vector<T> *p) {
    return G(p);
}

template<class T>
void RandomProjection<T>::generateRandVector() {
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
        r[i] = vector;
    }

}

template<class T>
RandomProjection<T>::RandomProjection(int d, int k):HashFunction<T>(d), k(k) {
    // create random r vectors
    r = new std::vector<double>*[k];
    generateRandVector();
}

template<class T>
int RandomProjection<T>::h(std::vector<T> *p, std::vector<double> *v) {
    double dot = Functions::dotProduct(*p, *v);
    return (dot >= 0 ? 1 : 0);
}

template<class T>
long RandomProjection<T>::G(std::vector<T> *p) {
    long result = h(p, r[0]);
    for (int i = 1; i < k; i++) {
        result = result<<1;
        int ht = h(p, r[i]);
        result |= ht;
    }
    return result;
}


#endif //PROJECT2018_RANDOPROJECTION_H
