//
// Created by chris on 11/25/2018.
//

#ifndef CLUSTER_CONFIGREADER_H
#define CLUSTER_CONFIGREADER_H

#include <string>
#include <map>

#include "CSVReader.h"

class ConfigReader{
public:
    ConfigReader(std::string input_file);
    std::string get(std::string conf);
private:
    std::map<std::string, std::string> configuration;
};


#endif //CLUSTER_CONFIGREADER_H
