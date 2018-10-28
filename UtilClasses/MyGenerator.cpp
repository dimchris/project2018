//
// Created by chris on 10/25/2018.
//

#include "MyGenerator.h"

MyGenerator *MyGenerator::instance = 0;

MyGenerator *MyGenerator::getInstance() {
    if (instance == 0) {
        instance = new MyGenerator();
    }

    return instance;
}

MyGenerator::MyGenerator() {
    generator = new std::default_random_engine;
}

std::default_random_engine *MyGenerator::getGenerator() {
    return generator;
}

MyGenerator::~MyGenerator() {
    delete generator;
    delete instance;
}

void MyGenerator::destroy() {
    if(instance != NULL){
        delete instance;
        instance = NULL;
    }
}
