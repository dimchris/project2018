//
// Created by chris on 11/25/2018.
//

#ifndef CLUSTER_CLUSTERINIT_H
#define CLUSTER_CLUSTERINIT_H

#include <vector>
#include <set>
#include "../../Model/MyVector.h"
#include "../../Metrics/Metric.h"
#include "../../UtilClasses/Compare.h"


template <class T>
class ClusterInit {
protected:
    int id;
public:
    int getId() const {
        return id;
    }

protected:
    int k;
public:
    ClusterInit(int k):k(k){};
    virtual std::set<MyVector<T> *, Compare<T> > getInit(std::vector<MyVector<T> *> *vector)=0;
};


#endif //CLUSTER_CLUSTERINIT_H
