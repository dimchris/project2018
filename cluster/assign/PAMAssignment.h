//
// Created by chris on 12/2/2018.
//

#ifndef CLUSTER_PAMASSIGNEMETNT_H
#define CLUSTER_PAMASSIGNEMETNT_H

#include <limits>
#include "Assignment.h"

template<class T>
class PAMAssignment : public Assignment<T> {
public:
    PAMAssignment(Metric<T> *metric) : Assignment<T>(metric) {Assignment<T>::id = 1;}

    void assign(std::vector<MyVector<T> *> *sample, std::vector<Cluster<T> *> *clusters) override {
        // init clusters
        for (int i = 0; i < clusters->size(); i++)
            clusters->at(i)->init();
        typename std::vector<MyVector<T> *>::iterator it;
        for (it = sample->begin(); it != sample->end(); ++it) {
            if(isCendroid(*it, clusters))
                continue;
            double distance0 = std::numeric_limits<double>::max();
            Cluster<T> *closest = NULL;
            for (int i = 0; i < clusters->size(); i++) {
                double distance = Assignment<T>::metric->distance(*it, clusters->at(i)->getCentroid());
                if (distance < distance0) {
                    distance0 = distance;
                    closest = clusters->at(i);
                }
            }
            closest->assign(*it);
            assert((*it)->getSize() == 203);
        }
        int total = 0;
        for (int i = 0; i < clusters->size(); i++) {
            total += clusters->at(i)->getAssigned()->size();
        }
    }

private:
    bool isCendroid(MyVector<T> *vector, std::vector<Cluster<T> *> *clusters) {
        for (int i = 0; i < clusters->size(); ++i) {
            if (clusters->at(i)->getCentroid()->getId() == vector->getId())
                return true;
        }
        return false;
    }
};


#endif //CLUSTER_PAMASSIGNEMETNT_H
