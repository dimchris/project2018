//
// Created by chris on 10/28/2018.
//

#ifndef PROJECT2018_NEIGHBOR_H
#define PROJECT2018_NEIGHBOR_H


#include "MyVector.h"
template<class T>
class Neighbor {
private:
    MyVector<T> *vector;
public:
    MyVector<T> *getVector() const {
        return vector;
    }

private:
    double distance;
public:

    Neighbor():distance(0), vector(NULL) {}
    Neighbor(MyVector<T> *vector, double distance):vector(vector), distance(distance){}

    double getDistance() const {
        return distance;
    }
};


#endif //PROJECT2018_NEIGHBOR_H
