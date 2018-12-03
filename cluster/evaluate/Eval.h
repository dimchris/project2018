//
// Created by chris on 12/1/2018.
//

#ifndef CLUSTER_EVAL_H
#define CLUSTER_EVAL_H
#include "../Cluster.h"
#include "../../Metrics/Metric.h"

template <class T>
class Eval {
protected:
    Metric<T> *metric;
    Eval(Metric<T> *metric):metric(metric){}

public:
    virtual double eval (std::vector<Cluster<T>*> *clusters) = 0;
};


#endif //CLUSTER_EVAL_H
