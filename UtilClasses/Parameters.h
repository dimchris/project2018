//
// Created by chris on 10/28/2018.
//

#ifndef PROJECT2018_PARAMETERS_H
#define PROJECT2018_PARAMETERS_H


#include <string>
#include <map>
class Parameters {
private:
    std::map<std::string,std::string> params;
public:
    Parameters(int argc, char **argv);
    std::string getParam(std::string key);


};


#endif //PROJECT2018_PARAMETERS_H
