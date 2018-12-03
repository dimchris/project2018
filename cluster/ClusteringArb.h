//
// Created by chris on 12/1/2018.
//

#ifndef CLUSTER_CLUSTERINGARB_H
#define CLUSTER_CLUSTERINGARB_H

#include "Clustering.h"

template <class T>
class ClusteringArb : public Clustering<T>{
public:
    ClusteringArb(std::vector<MyVector<double> *> *sample, ClusterInit<T> *init, Assignment<T> *assignment,
    Update<T> *update, Eval<T> *eval, int k, double tol) : Clustering<T>(sample, init, assignment, update,
                                                                         eval, k, tol) {}

    void initialize() override {

        const std::set<MyVector<T> *, Compare<T >> centroids = Clustering<T>::init->getInit(Clustering<T>::sample);

        //init clusters
        Clustering<T>::clusters = new std::vector<Cluster<double> *>(Clustering<T>::k);
        for (int i = 0; i < centroids.size(); i++) {
            typename std::set<MyVector<double> *>::iterator it;
            it = centroids.begin();
            advance(it, i);
            Clustering<T>::clusters->at(i) = new Cluster<double>(*it);
        }
    }

    void assignClusters() override {
        // no need to exclute the centroids. passing the whole sample
        Clustering<T>::assignment->assign(Clustering<T>::sample, Clustering<T>::clusters);
        Clustering<T>::eval0 = Clustering<T>::eval->eval(Clustering<T>::clusters);
    }

    void updateClusters() override {
        Clustering<T>::update->update(Clustering<T>::clusters);
        Clustering<T>::eval1 = Clustering<T>::eval->eval(Clustering<T>::clusters);
    }
};


#endif //CLUSTER_CLUSTERINGARB_H
