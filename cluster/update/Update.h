//
// Created by chris on 12/1/2018.
//

#ifndef CLUSTER_UPDATE_H
#define CLUSTER_UPDATE_H
#include "../Cluster.h"

template <class T>
class Update {
protected:
    int id;
public:
    int getId() const {
        return id;
    }

public:
    virtual void update(std::vector<Cluster<T>*> *clusters) = 0;
};


#endif //CLUSTER_UPDATE_H
