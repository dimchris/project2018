//
// Created by chris on 10/27/2018.
//

#ifndef PROJECT2018_EUCLIDEANLSH_H
#define PROJECT2018_EUCLIDEANLSH_H


#include "LSH.h"
#include "../Metrics/EuclideanMetric.h"

template<class T>
class EuclideanLSH : public LSH<T> {
public:
    EuclideanLSH(int d, int l, int tableSize, HashFunction<T> **hashFunction, double epsilon) : LSH<T>(d, l, tableSize, hashFunction,
                                                                                    new EuclideanMetric<T>(), epsilon) {};

};


#endif //PROJECT2018_EUCLIDEANLSH_H
