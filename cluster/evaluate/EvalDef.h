//
// Created by chris on 12/1/2018.
//

#ifndef CLUSTER_EVALDEF_H
#define CLUSTER_EVALDEF_H


#include "Eval.h"
template <class T>
class EvalDef : public Eval<T>{
public:
    EvalDef(Metric<T> *metric):Eval<T>(metric){}
    double eval(std::vector<Cluster<T> *> *clusters) override {
        double sum = 0;
        for (int i = 0; i < clusters->size(); ++i) {
            std::set<MyVector<T> *, Compare<T>> *assigned = clusters->at(i)->getAssigned();
            MyVector<T> * centroid = clusters->at(i)->getCentroid();
            typename std::set<MyVector<T> *, Compare<T>>::iterator it;
            for (it = assigned->begin(); it != assigned->end(); ++it) {
                sum += Eval<T>::metric->distance(*it, centroid);
            }
        }
        return sum;
    }
};


#endif //CLUSTER_EVALDEF_H
