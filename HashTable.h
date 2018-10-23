//
// Created by chris on 10/21/2018.
//

#ifndef PROJECT2018_HASHTABLE_H
#define PROJECT2018_HASHTABLE_H

#include <list>
#include <cstdarg>
#include <vector>
#include <stdexcept>
#include <assert.h>
#include "HashFunction.h"

template<class T>
class HashTable {
private:
    int size;
    int d;
    std::list<std::vector<T>* > *vectors;
    HashFunction<T> *hashFunction;
public:
    HashTable(int size, int d, HashFunction<T> *hashFunction) : size(size), d(d), hashFunction(
            hashFunction) {
        vectors = new std::list<std::vector<T>* >[size];
    };

    ~HashTable() { delete[] vectors; }

    void addPoint(std::vector<T> *p);

    std::list<std::vector<T>* > getBucket(int key);
};

template<class T>
void HashTable<T>::addPoint(std::vector<T> *p) {
    // get the key using the hash function
    int key = hashFunction->getKey(*p);
    assert(key<size);
    //add the point to the key bucket
    vectors[key].push_back(p);
}

template<class T>
std::list<std::vector<T>* > HashTable<T>::getBucket(int key) {
    assert(key > 0);
    assert(key < size);
    return vectors[key];
}


#endif //PROJECT2018_HASHTABLE_H
