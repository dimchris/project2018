//
// Created by chris on 10/27/2018.
//

#ifndef PROJECT2018_QUERYREADER_H
#define PROJECT2018_QUERYREADER_H


#include "DataReader.h"
#include "../Model/MyQuery.h"

template <class T>
class QueryReader : public CSVReader{
private:
    T getValue(std::string &val);
    double r;
public:
    QueryReader(std::string filename, char delimiter):CSVReader(filename, delimiter, true),r(0){
        std::vector<std::string> *values = new std::vector<std::string>;
        *values = Utils::split(*CSVReader::header, '=');
        if (values->size() > 1) {
            r = std::stod(values->at(1));
        }
        delete values;
    }
    MyQuery<T> * getQuery();

};

template<class T>
MyQuery<T> * QueryReader<T>::getQuery() {
    std::vector<std::string> *record;
    record = CSVReader::getRecord();
    if (record == NULL) {
        return NULL;
    }
    MyVector<T> *myvector = NULL;
    MyQuery<T> *myQuery = NULL;
    std::vector<int> *vector = new std::vector<int>();
    std::string id = record->at(0);
    for (int j = 1; j <record->size(); j++) { //dimension + one col for id
        vector->push_back(getValue(record->at(j)));
    }
    myvector = new MyVector<T>(id, vector);

    myQuery = new MyQuery<T>(myvector, this->r);

    delete record;

    return myQuery;
}

template<>
int QueryReader<int>::getValue(std::string &val) {
    return std::stoi(val);
}
template<>
double QueryReader<double>::getValue(std::string &val) {
    return std::stod(val);
}


#endif //PROJECT2018_QUERYREADER_H
