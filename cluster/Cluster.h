//
// Created by chris on 11/25/2018.
//

#ifndef CLUSTER_CLUSTER_H
#define CLUSTER_CLUSTER_H

#include <set>
#include "../Model/MyVector.h"
#include "../UtilClasses/Compare.h"

template <class T>
class Cluster {
private:
    MyVector<T> *centroid;
    std::vector<double> *sil;
public:
    std::vector<double> *getSil() const;
    double getMeanSil();

    void setSil(std::vector<double> *sil);

public:
    void setCentroid(MyVector<T> *centroid);

public:
    MyVector<T> *getCentroid() const;

private:
    std::set<MyVector<T> *, Compare<T> > *assigned;
public:
    void setAssigned(std::set<MyVector<T> *, Compare<T>> *assigned);

public:
    Cluster(MyVector<T> *cendroid);
    ~Cluster();

    std::set<MyVector<T> *, Compare<T> > *getAssigned();

    void assign(MyVector<T> *vector);

    void init();
};

template<class T>
Cluster<T>::Cluster(MyVector<T> *cendroid):centroid(cendroid), sil(NULL){
    assigned = new std::set<MyVector<T> *, Compare<T> >();
}

template<class T>
Cluster<T>::~Cluster() {
//    delete assigned;
}

template<class T>
void Cluster<T>::assign(MyVector<T> *vector) {
    assigned->insert(vector);
}

template<class T>
MyVector<T> *Cluster<T>::getCentroid() const {
    return centroid;
}

template<class T>
std::set<MyVector<T> *, Compare<T>> *Cluster<T>::getAssigned(){
    return assigned;
}

template<class T>
void Cluster<T>::init() {
    assigned->clear();
}

template<class T>
void Cluster<T>::setCentroid(MyVector<T> *centroid) {
    Cluster::centroid = centroid;
}

template<class T>
void Cluster<T>::setAssigned(std::set<MyVector<T> *, Compare<T>> *assigned) {
    Cluster::assigned = assigned;
}

template<class T>
std::vector<double> *Cluster<T>::getSil() const {
    return sil;
}

template<class T>
void Cluster<T>::setSil(std::vector<double> *sil) {
    Cluster::sil = sil;
}

template<class T>
double Cluster<T>::getMeanSil() {
    if(getAssigned()->size() == 0)
        return 1;
    double sum = 0.0;
    for (int i = 0; i < sil->size(); ++i) {
        sum += sil->at(i);
    }
    return sum/sil->size();
}


#endif //CLUSTER_CLUSTER_H
