//
// Created by chris on 10/27/2018.
//

#ifndef PROJECT2018_EUCLIDEANLHSINT_H
#define PROJECT2018_EUCLIDEANLHSINT_H


#include "../Hashing/HashFunction.h"
#include "EuclideanLSH.h"

class EuclideanLSHInt : public EuclideanLSH<int> {
public:
    EuclideanLSHInt(int d, int l, int tableSize, HashFunction<int> **hashFunction, double epsilon) : EuclideanLSH<int>(
            d, l, tableSize, hashFunction, epsilon) {}
};


#endif //PROJECT2018_EUCLIDEANLHSINT_H
