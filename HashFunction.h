//
// Created by chris on 10/21/2018.
//

#ifndef PROJECT2018_HASHFUNCTION_H
#define PROJECT2018_HASHFUNCTION_H

#include <vector>

template<class T>
class HashFunction {
public:
    HashFunction(int d) : d(d) {}

    virtual long getKey(std::vector<T> p) = 0;

protected:
    int d;

};


#endif //PROJECT2018_HASHFUNCTION_H
