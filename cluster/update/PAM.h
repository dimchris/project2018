//
// Created by chris on 12/1/2018.
//

#ifndef CLUSTER_PAM_H
#define CLUSTER_PAM_H


#include <limits>
#include "Update.h"
#include "../assign/Assignment.h"

template<class T>
class PAM : public Update<T> {
private:
    Metric<T> *metric;
public:
    PAM(Metric<T> *metric):metric(metric){Update<T>::id = 2;}
    void update(std::vector<Cluster<T> *> *clusters) override {
        for (int i = 0; i < clusters->size(); ++i) {
            std::set<MyVector<T> *, Compare<T>> *assigned = clusters->at(i)->getAssigned();
            //simple if convert to array
            std::vector<MyVector<T> *> *assignedVector = new std::vector<MyVector<T> *>(assigned->begin(),
                                                                                        assigned->end());
            double eval0 = objective(clusters->at(i));
            for (int j = 0; j < assignedVector->size(); ++j) {
                MyVector<T> *centroid = clusters->at(i)->getCentroid();
                // swap
                MyVector<T> *temp = assignedVector->at(j);
                assignedVector->at(j) = centroid;
                centroid = temp;
                // create a new cluster
                Cluster<T> *cluster = new Cluster<T>(centroid);
                cluster->setAssigned(
                        new std::set<MyVector<T> *, Compare<T>>(assignedVector->begin(), assignedVector->end()));
                double eval1 = objective(cluster);
                if (eval1 < eval0) {
                    delete clusters->at(i);
                    clusters->at(i) = cluster;
                    eval0 = eval1;
                    continue;
                }
                //swap back
                temp = assignedVector->at(j);
                assignedVector->at(j) = centroid;
                centroid = temp;
                //delete new cluster
                delete cluster;
            }

            delete assignedVector;
        }
    }

    double objective(Cluster<T> *cluster);

    void swap(MyVector<T> *vector, std::vector<MyVector<T> *> *assigned, int pos);
};

template<class T>
double PAM<T>::objective(Cluster<T> *cluster) {
    std::set<MyVector<T> *, Compare<T>> *assigned = cluster->getAssigned();
    typename std::set<MyVector<T> *, Compare<T>>::iterator it;
    double sum = 0.0;
    for (it = assigned->begin(); it != assigned->end(); ++it) {
        double distance = metric->distance(*it, cluster->getCentroid());
        sum += distance;
    }
    return sum;
}

template<class T>
void PAM<T>::swap(MyVector<T> *vector, std::vector<MyVector<T> *> *assigned, int pos) {
    MyVector<T> *temp = assigned->at(pos);
    assigned->at(pos) = vector;
    vector = temp;
}


#endif //CLUSTER_PAM_H
