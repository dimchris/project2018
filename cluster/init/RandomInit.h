//
// Created by chris on 11/25/2018.
//

#ifndef CLUSTER_RANDOMINIT_H
#define CLUSTER_RANDOMINIT_H


#include "ClusterInit.h"
#include "../../UtilClasses/MyGenerator.h"
#include "../../UtilClasses/Compare.h"
#include <set>
#include <iostream>
#include <assert.h>
#include <math.h>

template<class T>
class RandomInit : public ClusterInit<T> {
public:
    RandomInit(int k) : ClusterInit<T>(k) {ClusterInit<T>::id=1;}

    std::set<MyVector<T> *, Compare<T> > getInit(std::vector<MyVector<T> *> *vector) {
        // create a new set
        std::set<MyVector<T> *, Compare<T>> init;

        MyGenerator *myGenerator = MyGenerator::getInstance();
        std::uniform_real_distribution<double> distribution(0, vector->size() - 1);
        std::default_random_engine *generator = myGenerator->getGenerator();
        while (init.size() < ClusterInit<T>::k) {
            int rand = (int) std::floor(distribution(*generator));
            assert(rand < vector->size() && rand >= 0);
            init.insert(vector->at(rand));
        }
        return init;
    }
};


#endif //CLUSTER_RANDOMINIT_H
