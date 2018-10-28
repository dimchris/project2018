//
// Created by chris on 10/27/2018.
//

#ifndef PROJECT2018_DATAREADER_H
#define PROJECT2018_DATAREADER_H


#include "CSVReader.h"
#include "../Model/MyVector.h"
#include "../UtilClasses/Utils.h"

template<class T>
class DataReader : public CSVReader {
private:
    std::string *metric;
public:
    std::string *getMetric() const;

private:

    T getValue(std::string &val);

public:
    DataReader(std::string filename, char delimiter) : CSVReader(filename, delimiter, true), metric(NULL) {
        std::vector<std::string> *values = new std::vector<std::string>;
        *values = Utils::split(*CSVReader::header, ' ');
        if (values->size() > 1) {
            metric = new std::string(values->at(1));
        }
        delete values;
    };

    MyVector<T> *getDataRecord();

};

template<class T>
MyVector<T> *DataReader<T>::getDataRecord() {
    std::vector<std::string> *record;
    record = CSVReader::getRecord();
    if (record == NULL) {
        return NULL;
    }
    MyVector<T> *myvector = NULL;
    std::vector<int> *vector = new std::vector<int>();
    std::string id = record->at(0);
    for (int j = 1; j < record->size(); j++) { //dimension + one col for id
        vector->push_back(std::stoi(record->at(j)));
    }
    myvector = new MyVector<T>(id, vector);

    delete record;

    return myvector;
}

template<>
int DataReader<int>::getValue(std::string &val) {
    return std::stoi(val);
}

template<class T>
std::string *DataReader<T>::getMetric() const {
    return metric;
}

template<>
double DataReader<double>::getValue(std::string &val) {
    return std::stod(val);
}


#endif //PROJECT2018_DATAREADER_H
