//
// Created by chris on 10/29/2018.
//

#ifndef CUBE_HYPERCUBE_H
#define CUBE_HYPERCUBE_H

#include "../LSH/LSH.h"
#include "../UtilClasses/UsefullFunctions.h"
#include <math.h>

template<class T>
class HyperCube : public LSH<T> {
public:
    HyperCube(int d, int m, int dd, int probes, HashFunction<T> **hashFunction, Metric<T> *metric, double epsilon);

private:
    int m;
public:
    std::set<MyVector<T> *, Compare<T>> getNeighbors(MyVector<T> *vector) override;

    std::set<Neighbor<T> *, Compare<T>> getNeighbors(MyQuery<T> *query) override;

private:
    int dd;
    int probes;

    int calcTableSize(int dd);
};

template<class T>
int HyperCube<T>::calcTableSize(int dd) {
    return (int) pow(2, dd);
}

template<class T>
HyperCube<T>::HyperCube(int d, int m, int dd, int probes, HashFunction<T> **hashFunction, Metric<T> *metric,
                        double epsilon)
        : LSH<T>(d,
                 1,
                 calcTableSize(dd),
                 hashFunction,
                 metric,
                 epsilon), m(m), dd(dd), probes(probes) {}

template<class T>
std::set<MyVector<T> *, Compare<T>> HyperCube<T>::getNeighbors(MyVector<T> *vector) {
    // set of neighbors
    std::set<MyVector<T> *, Compare<T> > neighbors = std::set<MyVector<T> *, Compare<T> >();
    // get the first bucket
    std::list<MyVector<T> *> bucket;
    int key;
    try {
        bucket = LSH<T>::hashTables[0]->getBucket(vector);
        key = LSH<T>::hashTables[0]->getKey(vector);
    } catch (std::invalid_argument e) {
        throw;
    }
    // get first bucket neighbors
    typename std::list<MyVector<T> *>::const_iterator iterator;
    int i = 0;
    for (iterator = bucket.begin(); iterator != bucket.end(); ++iterator) {
        // if met m break
        if (i >= m)
            break;
        i++;
        neighbors.insert(*iterator);
    }

    // get near buckets neighbors
    int totalBucketsChecked = 1; // all ready checked one
    for (int i = 0; i < pow(2, dd); i++) {
        if (Functions::differAtOneBitPos(i, key)) {
            totalBucketsChecked++;
            try {
                bucket = LSH<T>::hashTables[0]->getBucket(i);
            } catch (std::invalid_argument e) {
                throw;
            }
            int j = 0;
            for (iterator = bucket.begin(); iterator != bucket.end(); ++iterator) {
                // if met m break
                if (j >= m) break;
                j++;
                neighbors.insert(*iterator);
            }
        }
        // if meet probe break
        if (totalBucketsChecked >= probes) break;
    }


    return neighbors;
}

template<class T>
std::set<Neighbor<T> *, Compare<T>> HyperCube<T>::getNeighbors(MyQuery<T> *query) {
    double r = query->getR();
    MyVector<T> *vector = query->getVector();

    // get all neighbors
    std::set<MyVector<T> *, Compare<T> > allNeighbors;
    try {
        allNeighbors = getNeighbors(vector);
    } catch (std::invalid_argument e) {
        throw;
    }
    std::set<Neighbor<T> *, Compare<T> > neighbors = std::set<Neighbor<T> *, Compare<T> >();
    if (r > 0) {
        // if within R check the distance
        typename std::set<MyVector<T> *>::const_iterator iterator;
        for (iterator = allNeighbors.begin(); iterator != allNeighbors.end(); ++iterator) {
            //calc distance using the metric
            double distance = LSH<T>::metric->distance(*iterator, vector);
            // keep only vectors within the appr diastance
            if (distance <= r * LSH<T>::c) {
                neighbors.insert(new Neighbor<T>(*iterator, distance));
            }
        }

    } else {
        double distance0 = std::numeric_limits<double>::max();
        MyVector<T> *closest = NULL;
        typename std::set<MyVector<T> *>::const_iterator iterator;
        for (iterator = allNeighbors.begin(); iterator != allNeighbors.end(); ++iterator) {
            //calc distance using the metric
            double distance = LSH<T>::metric->distance(*iterator, vector);
            // keep only vectors within the appr diastance
            if (distance <= distance0 * LSH<T>::c) {
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

#endif //CUBE_HYPERCUBE_H
