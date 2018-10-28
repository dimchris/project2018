//
// Created by chris on 10/28/2018.
//

#include "Timer.h"

using namespace std;

void Timer::startTimer() {
    start = clock();
}

double Timer::stopTimer() {
    end = clock();
    return double(end - start) / CLOCKS_PER_SEC;
}

void Timer::reset() {
    start = 0;
    end = 0;
}
