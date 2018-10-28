//
// Created by chris on 10/28/2018.
//

#ifndef PROJECT2018_COMPARE_H
#define PROJECT2018_COMPARE_H


#include "../Model/MyVector.h"
#include "../Model/Neighbor.h"

template <class T>
class Compare {
public:
    bool operator ()( const MyVector<T> *p1,
                      const MyVector<T> *p2 ) const
    {
        bool test  =  p1->getId() < p2->getId();
        return ( p1->getId() < p2->getId() );
    }
    bool operator ()( const Neighbor<T> *p1,
                      const Neighbor<T> *p2 ) const
    {
        return ( p1->getDistance() < p2->getDistance() );
    }

};


#endif //PROJECT2018_COMPARE_H
