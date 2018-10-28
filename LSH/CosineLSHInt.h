//
// Created by chris on 10/28/2018.
//

#ifndef PROJECT2018_COSINELSHINT_H
#define PROJECT2018_COSINELSHINT_H


#include "CosineLSH.h"

class CosineLSHInt : public CosineLSH<int> {
public:
    CosineLSHInt(int d, int l, int tableSize, HashFunction<int> **hashFunction, double epsilon) : CosineLSH<int>(
            d, l, tableSize, hashFunction, epsilon) {}
};


#endif //PROJECT2018_COSINELSHINT_H
