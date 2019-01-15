//
// Created by chris on 10/25/2018.
//

#ifndef PROJECT2018_LHS_H
#define PROJECT2018_LHS_H

#include <list>
#include <vector>
#include <set>
#include "../Hashing/HashTable.h"
#include "../Model/MyQuery.h"
#include "../Metrics/Metric.h"
#include "../Model/Neighbor.h"
#include "../UtilClasses/Compare.h"
#include <limits>

template<class T>
class LSH {
private:
    int l;
    int d;
    int tableSize;
    HashFunction<T> **hashFunctions;
    std::list<MyVector<T> *> vectors;
    double epsilon;
protected:
    double c;
    Metric<T> *metric;
public:
    void setMetric(Metric<T> *metric);

public:
    Metric<T> *getMetric() const;

protected:
    HashTable<T> **hashTables;
public:
    LSH(int d, int l, int tableSize, HashFunction<T> **hashFunction, Metric<T> *metric, double epsilon);

    ~LSH();

    virtual std::set<MyVector<T> *, Compare<T> > getNeighbors(MyVector<T> *vector);

    virtual std::set<Neighbor<T> *, Compare<T> > getNeighbors(MyQuery<T> *query);

    void addPoint(MyVector<T> *vector);

    void setC(double c);

    void setEpsilon(double epsilon);

    Neighbor<T> *trueNearest(MyQuery<T> *q);

    int sizeOf();

};

template<class T>
LSH<T>::LSH(int d, int l, int tableSize, HashFunction<T> **hashFunction, Metric<T> *metric, double epsilon) :d(d), l(l),
                                                                                                             tableSize(
                                                                                                                     tableSize),
                                                                                                             hashFunctions(
                                                                                                                     hashFunction),
                                                                                                             c(1.0),
                                                                                                             metric(metric),
                                                                                                             epsilon(epsilon) {
    // init hashtables
    hashTables = new HashTable<T> *[l];
    for (int i = 0; i < l; i++) {
        try {
            hashTables[i] = new HashTable<T>(tableSize, d, hashFunction[i]);
        } catch (std::invalid_argument) {
            throw;
        }
    }
    this->c = 1 + epsilon;
}

template<class T>
LSH<T>::~LSH() {
    for (int i = 0; i < l; i++) {
        delete hashTables[i];
    }
    if (hashTables != NULL) {
        delete[] hashTables;
    }
    // delete vectors
//    typename std::list<MyVector<T> *>::const_iterator iterator;
//    for (iterator = vectors.begin(); iterator != vectors.end(); ++iterator) {
//        delete *iterator;
//    }
}

template<class T>
void LSH<T>::addPoint(MyVector<T> *vector) {
    // put point to vectors table
    vectors.push_back(vector);
    // push vector to each hashtable
    for (int i = 0; i < l; i++) {
        try {
            hashTables[i]->addPoint(vector);
        } catch (std::invalid_argument e) {
            throw;
        }

    }
}

template<class T>
std::set<MyVector<T> *, Compare<T> > LSH<T>::getNeighbors(MyVector<T> *vector) {
    // set of neighbors
    std::set<MyVector<T> *, Compare<T> > neighbors = std::set<MyVector<T> *, Compare<T> >();
    // for each hash table get the neighbors and add them to the set
    for (int i = 0; i < l; i++) {
        std::list<MyVector<T> *> bucket;
        try {
            bucket = hashTables[i]->getBucket(vector);
        } catch (std::invalid_argument e) {
            throw;
        }
        typename std::list<MyVector<T> *>::const_iterator iterator;
        for (iterator = bucket.begin(); iterator != bucket.end(); ++iterator) {
            neighbors.insert(*iterator);
        }
    }

    return neighbors;
}

template<class T>
std::set<Neighbor<T> *, Compare<T> > LSH<T>::getNeighbors(MyQuery<T> *query) {
    double r = query->getR();
    MyVector<T> *vector = query->getVector();

    // get all neighbors
    std::set<MyVector<T> *, Compare<T> > allNeighbors;
    try {
        allNeighbors = getNeighbors(vector);
    } catch (std::invalid_argument) {
        throw;
    }
    std::set<Neighbor<T> *, Compare<T> > neighbors = std::set<Neighbor<T> *, Compare<T> >();
    if (r > 0) {
        // if within R check the distance
        typename std::set<MyVector<T> *>::const_iterator iterator;
        for (iterator = allNeighbors.begin(); iterator != allNeighbors.end(); ++iterator) {
            //calc distance using the metric
            double distance = metric->distance(*iterator, vector);
            // keep only vectors within the appr diastance
            if (distance <= r * c) {
                neighbors.insert(new Neighbor<T>(*iterator, distance));
            }
        }

    } else {
        double distance0 = std::numeric_limits<double>::max();
        MyVector<T> *closest = NULL;
        typename std::set<MyVector<T> *>::const_iterator iterator;
        for (iterator = allNeighbors.begin(); iterator != allNeighbors.end(); ++iterator) {
            //calc distance using the metric
            double distance = metric->distance(*iterator, vector);
            // keep only vectors within the appr diastance
            if (distance <= distance0 * c) {
                closest = (*iterator);
                distance0 = distance;
            }
        }
        if (closest != NULL) {
            neighbors.insert(new Neighbor<T>(closest, distance0));
        }
    }
    return neighbors;
}

template<class T>
void LSH<T>::setC(double c) {
    this->c = c;
}

template<class T>
void LSH<T>::setEpsilon(double epsilon) {
    this->epsilon = epsilon;
    this->c = this->c + epsilon;
}

template<class T>
Neighbor<T> *LSH<T>::trueNearest(MyQuery<T> *q) {
    double distance0 = std::numeric_limits<double>::max();
    MyVector<T> *closest = NULL;
    double distance;

    typename std::list<MyVector<T> *>::const_iterator iterator;
    for (iterator = vectors.begin(); iterator != vectors.end(); ++iterator) {
        double distance = metric->distance(q->getVector(), *iterator);
        if (distance < distance0) {
            closest = *iterator;
            distance0 = distance;
        }
    }

    if (closest != NULL) {
        return new Neighbor<T>(closest, distance0);
    }
}

template<class T>
int LSH<T>::sizeOf() {
    int size = 0;
    for (int i = 0; i < i < l; ++i) {
        size += hashTables[i]->getSize();
    }
}

template<class T>
Metric<T> *LSH<T>::getMetric() const {
    return metric;
}

template<class T>
void LSH<T>::setMetric(Metric<T> *metric) {
    LSH::metric = metric;
}


#endif //PROJECT2018_LHS_H
