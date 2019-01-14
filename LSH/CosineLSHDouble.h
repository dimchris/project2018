//
// Created by chris on 1/12/2019.
//

#ifndef CLUSTER_COSINELSHDOUBLE_H
#define CLUSTER_COSINELSHDOUBLE_H


#include "CosineLSH.h"

class CosineLSHDouble : public CosineLSH<double > {
public:
    CosineLSHDouble(int d, int l, int tableSize, HashFunction<double> **hashFunction, double epsilon) : CosineLSH<double >(
            d, l, tableSize, hashFunction, epsilon) {}
};


#endif //CLUSTER_COSINELSHDOUBLE_H
