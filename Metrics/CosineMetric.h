//
// Created by chris on 10/28/2018.
//

#ifndef PROJECT2018_COSINEMETRIC_H
#define PROJECT2018_COSINEMETRIC_H


#include "Metric.h"
#include "../UtilClasses/UsefullFunctions.h"
#include <math.h>

template <class T>
class CosineMetric : public Metric<T>{
public:
    double distance(MyVector<T> *vector0, MyVector<T> *vector1) override {
        double value = Functions::dotProduct(*vector0->getVector(), *vector1->getVector());
        value/=sqrt(Functions::dotProduct(*vector0->getVector(), *vector0->getVector()));
        value/=sqrt(Functions::dotProduct(*vector1->getVector(), *vector1->getVector()));
        return value;
    }
};


#endif //PROJECT2018_COSINEMETRIC_H
