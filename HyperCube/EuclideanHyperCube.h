//
// Created by chris on 10/29/2018.
//

#ifndef CUBE_EUCLIDEANHYPERCUBE_H
#define CUBE_EUCLIDEANHYPERCUBE_H

#include "HyperCube.h"
#include "../Metrics/EuclideanMetric.h"

template <class T>
class EuclideanHyperCube : public HyperCube<T> {
public:
    EuclideanHyperCube(int d, int m, int dd, int probes, HashFunction<T> **hashFunction,
                       double epsilon) : HyperCube<T>(d, m, dd, probes, hashFunction, new EuclideanMetric<T>, epsilon) {}
};


#endif //CUBE_EUCLIDEANHYPERCUBE_H
