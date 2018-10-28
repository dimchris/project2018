//
// Created by chris on 10/24/2018.
//

#include "CSVReader.h"
#include "../UtilClasses/Utils.h"

std::vector<std::string> * CSVReader::getRecord() {
    std::string *line;
    if((line = FileReader::readLine())!= NULL){
        std::vector<std::string> *values = new std::vector<std::string>;
        *values = Utils::split(*line, delimiter);
        return values;
    }
    return NULL;
}

void CSVReader::close() {
    FileReader::close();
}
