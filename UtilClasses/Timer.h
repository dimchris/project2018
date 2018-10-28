//
// Created by chris on 10/28/2018.
//

#ifndef PROJECT2018_TIMER_H
#define PROJECT2018_TIMER_H


#include <ctime>

class Timer {
private:
    clock_t start;
    clock_t end;
public:
    void startTimer();
    double stopTimer();
    void reset();

public:
    Timer(): start(0.0), end(0.0){}
};


#endif //PROJECT2018_TIMER_H
