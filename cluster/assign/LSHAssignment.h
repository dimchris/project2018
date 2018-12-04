//
// Created by chris on 12/2/2018.
//

#ifndef CLUSTER_LSHASSIGNMENT_H
#define CLUSTER_LSHASSIGNMENT_H

#include <unordered_map>
#include "Assignment.h"
#include "../../LSH/LSH.h"

template<class T>
class LSHAssignment : public Assignment<T> {
private:
    LSH<T> *lsh;
public:
    LSHAssignment(LSH<T> *lsh) : Assignment<T>(lsh->getMetric()), lsh(lsh) {}

    void assign(std::vector<MyVector<T> *> *sample, std::vector<Cluster<T> *> *clusters) override {
        // init clusters
        for (int i = 0; i < clusters->size(); i++)
            clusters->at(i)->init();
        //calc the init radius
        double distance0 = std::numeric_limits<double>::max();
        // and get the centroids
        std::set<MyVector<T> *, Compare<T>> centroids;
        // set of vectors to be assigned
        std::set<MyVector<T> *, Compare<T>> assign;
        assign.insert(sample->begin(), sample->end());
        for (int i = 0; i < clusters->size(); ++i) {
            Cluster<T> *cluster_a = clusters->at(i);
            centroids.insert(cluster_a->getCentroid());
            for (int j = i + 1; j < clusters->size(); ++j) {
                Cluster<T> *cluster_b = clusters->at(j);
                double distance = lsh->getMetric()->distance(cluster_a->getCentroid(), cluster_b->getCentroid());
                if (distance < distance0 && distance != 0)
                    distance0 = distance;
            }
        }
        // remove centroids from assigned
        typename std::set<MyVector<T> *, Compare<T> >::iterator it;
        for (it = centroids.begin(); it != centroids.end(); ++it) {
            assign.erase(*it);
        }

        double radius = distance0 / 2;
        int total_assigned = 1;
        while (assign.size() > 0 && total_assigned != 0) {
            total_assigned = 0;
            // query for centroids until to more to be assigned
            //crete a map of all to be assigned
            std::unordered_map<MyVector<T> *, std::vector<MyVector<T> *> > to_be_asigned;
            std::set<MyVector<T> *, Compare<T>> to_be_removed;
            typename std::set<MyVector<T> *, Compare<T> >::iterator it;
            for (it = centroids.begin(); it != centroids.end(); ++it) {
                MyQuery<T> query = MyQuery<T>(*it, radius);
                // get the neighbor set
                std::set<Neighbor<T> *, Compare<T>> neighbors0 = lsh->getNeighbors(&query);
                if (neighbors0.size() == 0)
                    continue;
                //add to the map
                typename std::set<Neighbor<T> *, Compare<T> >::iterator it0;
                for (it0 = neighbors0.begin(); it0 != neighbors0.end(); ++it0) {
                    if (assign.find((*it0)->getVector()) != assign.end())
                        to_be_asigned[(*it0)->getVector()].push_back(*it);
                }
            }
            // check all the map
            for (auto const &x : to_be_asigned) {
                // if is not empty
                if (!x.second.empty()) {
                    // get the closest centroid
                    distance0 = std::numeric_limits<double>::max();
                    MyVector<T> *closest = NULL;
                    for (int j = 0; j < x.second.size(); j++) {
                        double distance = lsh->getMetric()->distance(x.second.at(j), x.first);
                        if (distance < distance0) {
                            distance0 = distance;
                            closest = x.second.at(j);
                        }
                    }
                    assert(closest != NULL);
                    // search for the cluster and assign
                    for (int i = 0; i < clusters->size(); ++i) {
                        if (clusters->at(i)->getCentroid() == closest) {
                            //assign
                            clusters->at(i)->assign(x.first);
                            total_assigned++;
                            // add to remove list
                            to_be_removed.insert(x.first);
                            break;
                        }
                    }
                }
            }

            // double the radius
            radius *= 2.0;
            // remove from the assign set everything that is already assigned
            typename std::set<MyVector<T> *, Compare<T> >::iterator it2;
            for (it2 = to_be_removed.begin(); it2 != to_be_removed.end(); ++it2) {
                int t = assign.erase(*it2);
            }
        }
        // if more to be assigned assign them to the nearest centroid
        if (assign.size() > 0) {
            typename std::set<MyVector<T> *, Compare<T> >::iterator it0;
            for (it0 = assign.begin(); it0 != assign.end(); ++it0) {
                // get the closest centroid
                distance0 = std::numeric_limits<double>::max();
                Cluster<T> *closest = NULL;
                for (int j = 0; j < clusters->size(); j++) {
                    double distance = lsh->getMetric()->distance(clusters->at(j)->getCentroid(), *it0);
                    if (distance < distance0) {
                        distance0 = distance;
                        closest = clusters->at(j);
                    }
                }
                closest->assign(*it0);
            }
        }
    }
};


#endif //CLUSTER_LSHASSIGNMENT_H
