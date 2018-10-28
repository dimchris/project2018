//
// Created by chris on 10/28/2018.
//

#include "Parameters.h"

Parameters::Parameters(int argc, char **argv) {
    for(int i=1; i<argc; i+=2){
        std::string key = std::string(argv[i]);
        std::string val = std::string(argv[i+1]);
        params[key] = val;
    }

}

std::string Parameters::getParam(std::string key) {
    return params[key];
}
