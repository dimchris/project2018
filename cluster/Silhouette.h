//
// Created by chris on 12/2/2018.
//

#ifndef CLUSTER_SILHOUETTE_H
#define CLUSTER_SILHOUETTE_H

#include <limits>
#include <assert.h>
#include "../Metrics/Metric.h"
#include "Cluster.h"

template<class T>
class Silhouette {
private:
    std::vector<Cluster<T> *> *clusters;
    std::vector<MyVector<T> *> *centroids;
    Metric<T> *metric;
    bool isEval;

    double eval(MyVector<T> *vector, Cluster<T> *cluster);

public:
    Silhouette(std::vector<Cluster<T> *> *clusters, Metric<T> *metric) : clusters(clusters), metric(metric), isEval(
            false) {
        centroids = new std::vector<MyVector<T> *>(clusters->size());
        for (int i = 0; i < clusters->size(); ++i) {
            centroids->at(i) = clusters->at(i)->getCentroid();
        }
    }

    void eval();

    double getMean();
};

template<class T>
double Silhouette<T>::eval(MyVector<T> *vector, Cluster<T> *cluster) {
    // find second closest
    double distance0, distance;
    distance0 = std::numeric_limits<double>::max();
    Cluster<T> *next = NULL;
    for (int i = 0; i < centroids->size(); ++i) {
        // if already in set continue
        if (cluster == clusters->at(i))
            continue;
        distance = metric->distance(centroids->at(i), vector);
        if (distance < distance0) {
            distance0 = distance;
            next = clusters->at(i);
        }
    }
    assert(cluster != NULL && next != NULL);
    // find the mean distance between same cluster
    double mean_a = 0.0;
    std::set<MyVector<T> *, Compare<T> > *assigned = cluster->getAssigned();
    typename std::set<MyVector<T> *, Compare<T> >::iterator it;
    for (it = assigned->begin(); it != assigned->end(); ++it) {
        if (vector == *it)
            continue;
        mean_a += metric->distance(*it, vector);
    }
    mean_a += metric->distance(vector, cluster->getCentroid());
    if (mean_a == 0.0)
        return 1;
    mean_a /= (assigned->size() + 1);
    // find the mean between next cluster
    double mean_b = 0.0;
    assigned = next->getAssigned();
    for (it = assigned->begin(); it != assigned->end(); ++it) {
        if (vector == *it)
            continue;
        mean_b += metric->distance(*it, vector);
    }
    mean_b += metric->distance(vector, next->getCentroid());
    mean_b /= (assigned->size() + 1);

    double max = mean_a > mean_b ? mean_a : mean_b;

    double result = (mean_b - mean_a) / max;

    return result;
}

template<class T>
void Silhouette<T>::eval() {
    isEval = true;
    for (int i = 0; i < clusters->size(); ++i) {
        std::vector<double> *sils = new std::vector<double>();
        std::set<MyVector<T> *, Compare<T> > *assigned = clusters->at(i)->getAssigned();
        typename std::set<MyVector<T> *, Compare<T> >::iterator it;
        for (it = assigned->begin(); it != assigned->end(); ++it) {
            double sil = eval(*it, clusters->at(i));
            sils->push_back(sil);
        }
        clusters->at(i)->setSil(sils);
    }
}

template<class T>
double Silhouette<T>::getMean() {
    if (!isEval)
        eval();
    double sum = 0.0;
    int total = 0;
    for (int i = 0; i < clusters->size(); ++i) {
        if(clusters->at(i)->getAssigned()->size() == 0)
            sum += 1;
        for (int j = 0; j < clusters->at(i)->getSil()->size(); ++j) {
            total++;
            sum += clusters->at(i)->getSil()->at(j);
        }
    }
    return sum / total;
}


#endif //CLUSTER_SILHOUETTE_H
