//
// Created by chris on 10/27/2018.
//

#ifndef PROJECT2018_METRIC_H
#define PROJECT2018_METRIC_H


#include "../Model/MyVector.h"

template <class T>
class Metric {
public:
    virtual double distance(MyVector<T> *vector0, MyVector<T> *vector1) = 0;
};


#endif //PROJECT2018_METRIC_H
