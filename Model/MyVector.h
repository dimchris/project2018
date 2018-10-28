//
// Created by chris on 10/27/2018.
//

#ifndef PROJECT2018_MYVECTOR_H
#define PROJECT2018_MYVECTOR_H


#include <vector>
#include <string>
template <class T>
class MyVector {
private:
    std::string id;
    std::vector<T> *vector;
    int size;
public:
    const std::string &getId() const {
        return id;
    }

    void setId(const std::string &id) {
        MyVector::id = id;
    }

    std::vector<T> *getVector() const {
        return vector;
    }

    void setVector(std::vector<T> *vector) {
        MyVector::vector = vector;
    }
    int getSize(){ return size;}

    MyVector(std::string id, std::vector<T> *vector):id(id),vector(vector){size = vector->size();}
    ~MyVector(){delete vector;}

};


#endif //PROJECT2018_MYVECTOR_H
