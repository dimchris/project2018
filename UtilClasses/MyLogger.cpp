//
// Created by chris on 10/26/2018.
//

#include <iostream>
#include "MyLogger.h"

MyLogger *MyLogger::instance = 0;


const int MyLogger::NO_LOG = -1;
const int MyLogger::ERROR = 0;
const int MyLogger::LOG = 1;
const int MyLogger::WARN = 2;
const int MyLogger::INFO = 3;

MyLogger::MyLogger() {
    level = LOG; //default level
}

MyLogger *MyLogger::getInstance() {
    if (instance == 0) {
        instance = new MyLogger();
    }
    return instance;
}

MyLogger::~MyLogger() {
//    delete instance;
}

void MyLogger::log(std::string msg) {
    if (level > 0)
        std::cout << "LOG: " << msg << std::endl;
}

void MyLogger::warn(std::string msg) {
    if (level > 1)
        std::cout << "WARNING: " << msg << std::endl;
}

void MyLogger::info(std::string msg) {
    if (level > 2)
        std::cout << "INFO: " << msg << std::endl;

}

void MyLogger::error(std::string msg) {
    if (level > -1)
        std::cout << "ERROR: " << msg << std::endl;
}

void MyLogger::setLevel(int level) {
    this->level = level;
}

void MyLogger::destroy() {
    if(instance != NULL){
        delete instance;
        instance = NULL;
    }
}
