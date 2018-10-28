//
// Created by chris on 10/28/2018.
//

#ifndef PROJECT2018_COSINELSH_H
#define PROJECT2018_COSINELSH_H

#include "LSH.h"
#include "../Metrics/CosineMetric.h"

template <class T>
class CosineLSH :public LSH<T>{
public:
    CosineLSH(int d, int l, int tableSize, HashFunction<T> **hashFunction, double epsilon) : LSH<T>(d, l, tableSize, hashFunction,
                                                                                                       new CosineMetric<T>(), epsilon) {};
};


#endif //PROJECT2018_COSINELSH_H
