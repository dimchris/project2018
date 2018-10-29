//
// Created by chris on 10/29/2018.
//

#ifndef CUBE_RANDOMPROJECTIONCUBE_H
#define CUBE_RANDOMPROJECTIONCUBE_H


#include "RandomProjection.h"
template <class T>
class RandomProjectionCube: public RandomProjection<T> {
public:
    RandomProjectionCube(int d, int dd) : RandomProjection<T>(d, dd) {}
};


#endif //CUBE_RANDOMPROJECTIONCUBE_H
