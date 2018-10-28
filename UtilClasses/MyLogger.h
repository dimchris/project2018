//
// Created by chris on 10/26/2018.
//

#ifndef PROJECT2018_MYLOGGER_H
#define PROJECT2018_MYLOGGER_H


#include <string>

class MyLogger {
private:
    int level;
    static MyLogger *instance;
    MyLogger();
public:
    static MyLogger *getInstance();
    static const int NO_LOG;
    static const int LOG;
    static const int INFO;
    static const int WARN;
    static const int ERROR;
    ~MyLogger();
    void destroy();


     void log(std::string msg);

     void warn(std::string msg);

     void info(std::string msg);

     void error(std::string msg);

    void setLevel(int level);

};


#endif //PROJECT2018_MYLOGGER_H
