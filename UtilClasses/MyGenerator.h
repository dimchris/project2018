//
// Created by chris on 10/25/2018.
//

#ifndef PROJECT2018_MYGENERATOR_H
#define PROJECT2018_MYGENERATOR_H


#include <random>

class MyGenerator {
private:
    std::default_random_engine *generator;
    static MyGenerator* instance;

    MyGenerator();

public:
    static MyGenerator *getInstance();
    std::default_random_engine *getGenerator() ;
    ~MyGenerator();
    static void destroy();
};


#endif //PROJECT2018_MYGENERATOR_H
