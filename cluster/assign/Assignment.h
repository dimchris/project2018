//
// Created by chris on 11/26/2018.
//

#ifndef CLUSTER_ASSIGNMENT_H
#define CLUSTER_ASSIGNMENT_H

#include "../Cluster.h"
#include "../../UtilClasses/Compare.h"
#include "../../Metrics/Metric.h"

template<class T>
class Assignment {
protected:
    Metric<T> *metric;
    int id;
public:
    int getId() const {
        return id;
    }

public:
    Assignment(Metric<T> *metric):metric(metric){}
    virtual void assign(std::vector<MyVector<T> *> *sample, std::vector<Cluster<T>*> *clusters) = 0;
};


#endif //CLUSTER_ASSIGNMENT_H
