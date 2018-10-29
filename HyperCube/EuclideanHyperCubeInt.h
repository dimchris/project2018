//
// Created by chris on 10/29/2018.
//

#ifndef CUBE_EUCLIDEANHYPERCUBEINT_H
#define CUBE_EUCLIDEANHYPERCUBEINT_H


#include "EuclideanHyperCube.h"

class EuclideanHyperCubeInt : public EuclideanHyperCube<int>{

public:
    EuclideanHyperCubeInt(int d, int m, int dd, int probes, HashFunction<int> **hashFunction, double epsilon);
};


#endif //CUBE_EUCLIDEANHYPERCUBEINT_H
