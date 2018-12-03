//
// Created by chris on 12/1/2018.
//

#ifndef CLUSTER_CLUSTERINGVECTOR_H
#define CLUSTER_CLUSTERINGVECTOR_H

#include "Clustering.h"

template <class T>
class ClusteringVector : public Clustering<T>{
public:
    ClusteringVector(std::vector<MyVector<double> *> *sample, ClusterInit<T> *init, Assignment<T> *assignment,
                     Update<T> *update, Eval<T> *eval, int k, double tol) : Clustering<T>(sample, init, assignment, update,
                                                                                       eval, k, tol) {}

    void initialize() override {

        const std::set<MyVector<T> *, Compare<T >> centroids = Clustering<T>::init->getInit(Clustering<T>::sample);

        //init clusters
        Clustering<T>::clusters = new std::vector<Cluster<T> *>(Clustering<T>::k);
        for (int i = 0; i < centroids.size(); i++) {
            typename std::set<MyVector<T> *>::iterator it;
            it = centroids.begin();
            advance(it, i);

            // create new item
            std::vector<T> *vector0 = new std::vector<T>;
            std::vector<T> *vector = (*it)->getVector();

            for (int j = 0; j < vector->size(); ++j) {
                vector0->push_back(vector->at(j));
            }

            MyVector<T> *centroid = new MyVector<T>("centroid_" + std::to_string(i), vector0);

            Clustering<T>::clusters->at(i) = new Cluster<T>(centroid);

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


#endif //CLUSTER_CLUSTERINGVECTOR_H
