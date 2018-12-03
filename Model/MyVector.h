//
// Created by chris on 10/27/2018.
//

#ifndef PROJECT2018_MYVECTOR_H
#define PROJECT2018_MYVECTOR_H


#include <vector>
#include <string>

template<class T>
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

    int getSize() const { return size; }

    MyVector(std::string id, std::vector<T> *vector) : id(id), vector(vector) { size = vector->size(); }

    ~MyVector() { delete vector; }

    inline void add(const MyVector<T> &a);


};

template<>
inline void MyVector<double>::add(const MyVector<double> &a) {
    for (int j = 0; j < this->getSize(); ++j) {
        this->vector->at(j) +=  a.getVector()->at(j);
    }
}
template<>
inline void MyVector<int>::add(const MyVector<int> &a) {
    for (int j = 0; j < this->getSize(); ++j) {
        this->vector->at(j) +=  a.getVector()->at(j);
    }
}


#endif //PROJECT2018_MYVECTOR_H
