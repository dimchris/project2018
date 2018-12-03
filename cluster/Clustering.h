//
// Created by chris on 12/1/2018.
//

#ifndef CLUSTER_CLUSTERING_H
#define CLUSTER_CLUSTERING_H

#include <stdexcept>
#include <iostream>
#include "init/ClusterInit.h"
#include "assign/Assignment.h"
#include "update/Update.h"
#include "evaluate/Eval.h"

template<class T>
class Clustering {
protected:
    int k;
    double eval0;
    double eval1;
    double tol;
    bool initialized;
    std::vector<MyVector<T> *> *sample;
    std::vector<Cluster<T> *> *clusters;
public:
    std::vector<Cluster<T> *> *getClusters() const;

protected:
    ClusterInit<T> *init;
    Assignment<T> *assignment;
    Update<T> *update;
    Eval<T> *eval;
public:
    Clustering(std::vector<MyVector<T> *> *sample, ClusterInit<T> *init, Assignment<T> *assignment,
               Update<T> *update, Eval<T> *eval, int k, double tol);

    virtual void initialize() = 0;

    virtual void assignClusters() = 0;

    virtual void updateClusters() = 0;

    virtual void evaluate();

    double step();

    void run();

};

template<class T>
Clustering<T>::Clustering(std::vector<MyVector<T> *> *sample, ClusterInit<T> *init, Assignment<T> *assignment,
                          Update<T> *update, Eval<T> *eval, int k, double tol)
        :sample(sample), init(init), assignment(assignment), update(update), eval(eval), k(k), tol(tol) {
    clusters = new std::vector<Cluster<double> *>(k);
}

template<class T>
double Clustering<T>::step() {
    if(initialized){
        assignClusters();
        updateClusters();
        double diff = eval1 - eval0;
        evaluate();
        return diff;
    }
    throw std::runtime_error("not initialized. plz initialize first");
}

template<class T>
void Clustering<T>::evaluate() {
    eval0 = eval1;
}

template<class T>
void Clustering<T>::run() {
    initialize();
    bool stop = false;
    while(!stop){
        double diff = step();
        if(abs(diff)<tol)
            stop = true;
    }
}

template<class T>
std::vector<Cluster<T> *> *Clustering<T>::getClusters() const {
    return clusters;
}


#endif //CLUSTER_CLUSTERING_H
