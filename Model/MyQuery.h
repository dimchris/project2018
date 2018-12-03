//
// Created by chris on 10/27/2018.
//

#ifndef PROJECT2018_MYQUERY_H
#define PROJECT2018_MYQUERY_H


#include "MyVector.h"
template <class T>
class MyQuery {
private:
    MyVector<T> *vector;
    double r;
public:
    MyQuery(MyVector<T> *vector, double r):vector(vector),r(r){}

    MyVector<T> *getVector() const {
        return vector;
    }

    double getR() const {
        return r;
    };
    ~MyQuery(){}
};


#endif //PROJECT2018_MYQUERY_H
