//
// Created by chris on 10/29/2018.
//

#ifndef CUBE_COSINEHYPERCUBE_H
#define CUBE_COSINEHYPERCUBE_H

#include "HyperCube.h"
#include "../Metrics/CosineMetric.h"

template <class T>
class CosineHyperCube: public HyperCube<T> {
public:
    CosineHyperCube(int d, int m, int dd, int probes, HashFunction<T> **hashFunction,double epsilon)
            : HyperCube<T>(d, m, dd, probes, hashFunction, new CosineMetric<T>(), epsilon) {}
};


#endif //CUBE_COSINEHYPERCUBE_H
