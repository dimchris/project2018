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
#include "../Model/MyVector.h"
#include <iostream>
#include <typeinfo>

template<class T>
class HashTable {
private:
    int size;
public:
    int getSize() const;

private:
    int d;
    std::list<MyVector<T> *> *vectors;
    HashFunction<T> *hashFunction;
public:
    HashTable(int size, int d, HashFunction<T> *hashFunction) : size(size), d(d), hashFunction(
            hashFunction) {
        if (hashFunction->getD() != d) {
            throw std::invalid_argument("Incompatible dimensions: hashtable, hashFunction");
        }
        vectors = new std::list<MyVector<T> *>[size];
    };

    ~HashTable();

    void empty();

    void addPoint(MyVector<T> *p);

    std::list<MyVector<T> *> getBucket(int key);

    std::list<MyVector<T> *> getBucket(MyVector<T> *vector);


    int getKey(MyVector<T> *vector);
};

template<class T>
void HashTable<T>::addPoint(MyVector<T> *p) {
    // get the key using the hash function
    int key;
    try {
        key = hashFunction->getKey(p->getVector());
    } catch (std::invalid_argument e) {
        throw;
    }
    assert(key < size);
    //add the point to the key bucket
    vectors[key].push_back(p);
}

template<class T>
std::list<MyVector<T> *> HashTable<T>::getBucket(int key) {
    assert(key >= 0);
    assert(key < size);
    return vectors[key];
}

template<class T>
void HashTable<T>::empty() {
    for (int i = 0; i < size; i++) {
        if (!vectors[i].empty()) {
            typename std::list<MyVector<T> *>::const_iterator iterator;
            for (iterator = vectors[i].begin(); iterator != vectors[i].end(); ++iterator) {
                delete *iterator;
            }
        }
    }
}

template<class T>
HashTable<T>::~HashTable() {
    //do not empty cause keep vector pointer to different tables
//    empty();
    delete[] vectors;
}

template<class T>
int HashTable<T>::getKey(MyVector<T> *vector) {
    try {
        return hashFunction->getKey(vector->getVector());
    } catch (std::invalid_argument e) {
        throw;
    }
}

template<class T>
std::list<MyVector<T> *> HashTable<T>::getBucket(MyVector<T> *vector) {
    return getBucket(getKey(vector));
}

template<class T>
int HashTable<T>::getSize() const {
    return size;
}


#endif //PROJECT2018_HASHTABLE_H
