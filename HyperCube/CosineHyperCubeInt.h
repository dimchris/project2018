//
// Created by chris on 10/29/2018.
//

#ifndef CUBE_COSINEHYPERCUBEINT_H
#define CUBE_COSINEHYPERCUBEINT_H


#include "CosineHyperCube.h"

class CosineHyperCubeInt : public CosineHyperCube<int>{
public:
    CosineHyperCubeInt(int d, int m, int dd, int probes, HashFunction<int> **hashFunction, double epsilon)
            : CosineHyperCube(d, m, dd, probes, hashFunction, epsilon) {}
};


#endif //CUBE_COSINEHYPERCUBEINT_H
