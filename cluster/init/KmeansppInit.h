//
// Created by chris on 11/25/2018.
//

#ifndef CLUSTER_KMEANSPPINIT_H
#define CLUSTER_KMEANSPPINIT_H

#include "ClusterInit.h"
#include "../../UtilClasses/MyGenerator.h"
#include <iostream>
#include <chrono>

template<class T>
class KmeansppInit : public ClusterInit<T> {
private:
    Metric<T> *metric;

    double calcD(MyVector<T> *vector, std::set<MyVector<T> *, Compare<T> > centroids);

    std::vector<double> calcProb(std::set<MyVector<T> *, Compare<T> > centroids,
                                 std::set<MyVector<T> *, Compare<T> > non_centroids);

public:
    KmeansppInit(int k, Metric<T> *metric) : ClusterInit<T>(k), metric(metric) { ClusterInit<T>::id=2;};

    std::set<MyVector<T> *, Compare<T>> getInit(std::vector<MyVector<T> *> *vector) override {
        int t = 0;
        int n = vector->size();
        std::set<MyVector<T> *, Compare<T> > centroids;
        std::set<MyVector<T> *, Compare<T> > non_centroids;
        non_centroids.insert(vector->begin(), vector->end());
        MyGenerator *myGenerator = MyGenerator::getInstance();
        std::uniform_real_distribution<double> distribution(0, n - 1);
        std::default_random_engine *generator = myGenerator->getGenerator();
        // select the first centroid randomly
        typename std::set<MyVector<T> *>::iterator it;
        it = non_centroids.begin();
        advance(it, floor(distribution(*generator)));
        MyVector<T> *centroid = *it;

        // insert to centroids
        centroids.insert(centroid);
        //remove from centroids
        non_centroids.erase(centroid);

        // construct a trivial random generator engine from a time-based seed:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator0 (seed);

        for(int i =1; i<ClusterInit<T>::k; i++){
            //calc discrete propbs
            std::vector<double> probs = calcProb(centroids, non_centroids);
            std::discrete_distribution<int> dist = std::discrete_distribution<int>(probs.begin(), probs.end());
            // get next cendroid
            int centroidIndex = dist(generator0);
            it = non_centroids.begin();
            advance(it, centroidIndex);
            centroid = *it;
            // insert to centroids
            centroids.insert(centroid);
            //remove from centroids
            non_centroids.erase(centroid);
        }

        return centroids;

    };

};

template<class T>
double KmeansppInit<T>::calcD(MyVector<T> *vector, std::set<MyVector<T> *, Compare<T>> centroids) {
    typename std::set<MyVector<T> *>::iterator it;
    double distance0 = std::numeric_limits<double>::max();
    for (it = centroids.begin(); it != centroids.end(); ++it) {
        double distance = metric->distance(*it, vector);
        distance0 = distance < distance0 ? distance : distance0;
    }
    return distance0;

}

template<class T>
std::vector<double> KmeansppInit<T>::calcProb(std::set<MyVector<T> *, Compare<T>> centroids,
                                              std::set<MyVector<T> *, Compare<T>> non_centroids) {
    std::vector<double> probs(non_centroids.size());
    typename std::set<MyVector<T> *>::iterator it;
    int i = 0;
    double sum = 0.0;
    for (it = non_centroids.begin(); it != non_centroids.end(); ++it) {
        probs[i] = pow(calcD(*it, centroids), 2);
        sum = sum + probs[i];
        i++;
    }
    // normalization
    for (int i = 0; i < non_centroids.size(); i++) {
        probs[i] = probs[i] / sum;
    }

    return probs;
}


#endif //CLUSTER_KMEANSPPINIT_H
