//
// Created by chris on 10/27/2018.
//

#ifndef PROJECT2018_EUCLEDIANMETRIC_H
#define PROJECT2018_EUCLEDIANMETRIC_H


#include <stdexcept>
#include <math.h>
#include "Metric.h"
#include <assert.h>

template<class T>
class EuclideanMetric : public Metric<T> {
    double distance(MyVector<T> *vector0, MyVector<T> *vector1) override {
        if (vector0->getSize() != vector1->getSize()) {
            throw std::invalid_argument("Euclidean distance: incompatible vector dimensions");
        }
        double dist = 0.0;
        for (int i = 0; i < vector0->getSize(); i++) {
            dist += pow(1.0 * vector0->getVector()->at(i) - 1.0 * vector1->getVector()->at(i), 2);
        }

        assert(dist > 0);

        return sqrt(dist);
    }

};


#endif //PROJECT2018_EUCLEDIANMETRIC_H
