//
// Created by chris on 10/28/2018.
//

#include "Stats.h"

void Stats::update(double distance, double realDistance, double time) {
    // increase total
    total++;
    // calc ratio
    double ratio = distance/realDistance;
    //keep max
    this->ratio = (ratio>this->ratio?ratio:this->ratio);
    // add time
    this->time += time;
}

double Stats::getMeanTime() const {
    return time/total;
}

std::ostream &operator<<(std::ostream &os, const Stats &stats) {
    os << "MaxRatio: " << stats.ratio << " MeanTime: " << stats.getMeanTime();
    return os;
}

void Stats::reset() {
    time = 0.0;
    ratio = 0.0;
    total = 0;
}
