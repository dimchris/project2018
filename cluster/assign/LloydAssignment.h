//
// Created by chris on 11/26/2018.
//

#ifndef CLUSTER_LLOYDASSIGNMENT_H
#define CLUSTER_LLOYDASSIGNMENT_H


#include <limits>
#include "Assignment.h"

template<class T>
class LloydAssignment : public Assignment<T> {

public:
    LloydAssignment(Metric<T> *metric) : Assignment<T>(metric) { Assignment<T>::id = 1; }

    void assign(std::vector<MyVector<T> *> *sample, std::vector<Cluster<T> *>

    *clusters) override {
        // init clusters
        for (int i = 0; i < clusters->size(); i++)
            clusters->at(i)->init();
        // and get the centroids
        std::set<MyVector<T> *, Compare<T>> centroids;
        // set of vectors to be assigned
        std::set<MyVector<T> *, Compare<T>> assign;

        for (int i = 0; i < clusters->size(); ++i) {
            Cluster<T> *cluster_a = clusters->at(i);
            centroids.insert(cluster_a->getCentroid());
        }

        // remove centroids from assigned
        typename std::set<MyVector<T> *, Compare<T> >::iterator it;
        for (it = centroids.begin(); it != centroids.end(); ++it) {
            assign.erase(*it);
        }

        assign.insert(sample->begin(), sample->end());
        for (it = assign.begin(); it != assign.end(); ++it) {
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
        }
        int total = 0;
        for (int i = 0; i < clusters->size(); i++) {
            total += clusters->at(i)->getAssigned()->size();
        }
    }
};


#endif //CLUSTER_LLOYDASSIGNMENT_H
