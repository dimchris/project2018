//
// Created by chris on 12/1/2018.
//

#ifndef CLUSTER_VUPDATE_H
#define CLUSTER_VUPDATE_H

#include "Update.h"
#include "../../UtilClasses/Compare.h"

template<class T>
class VUpdate : public Update<T> {
public:
    VUpdate() : Update<T>() { Update<T>::id = 1; }

    void update(std::vector<Cluster<T> *> *clusters) override {
        // for each cluster evaluate the new center
        for (int i = 0; i < clusters->size(); ++i) {
            //calc the new center of mass

            // init a zero vector
            int size = clusters->at(0)->getCentroid()->getSize();
            std::vector<T> *vector0 = new std::vector<T>(size);
            for (int j = 0; j < vector0->size(); ++j) {
                setVector(vector0, j, 0);
            }
            MyVector<T> *new_centroid = new MyVector<T>("cluster_" + std::to_string(i), vector0);
            typename std::set<MyVector<T> *, Compare<T>>::iterator it;
            std::set<MyVector<T> *, Compare<T>> *assigned = clusters->at(i)->getAssigned();
            int total = 0;
            for (it = assigned->begin(); it != assigned->end(); ++it) {
                new_centroid->add(**it);
                total++;
            }
            for (int k = 0; k < vector0->size(); ++k) {
                new_centroid->getVector()->at(k) /= total;
            }
            if(total > 0){
                // delete previous cendroid
                if (clusters->at(i)->getCentroid()->getId().find("centroid_") != 0)
                    delete clusters->at(i)->getCentroid();
                // set new centroid
                clusters->at(i)->setCentroid(new_centroid);
            }else{
                delete new_centroid;
                new_centroid = NULL;
            }
        }
    }

private:
    void setVector(std::vector<T> *vector, int pos, T value);
};

template<>
void VUpdate<int>::setVector(std::vector<int> *vector, int pos, int value) {
    vector->at(pos) = value;
}

template<>
void VUpdate<double>::setVector(std::vector<double> *vector, int pos, double value) {
    vector->at(pos) = value;
}


#endif //CLUSTER_VUPDATE_H
