//
// Created by chris on 10/28/2018.
//

#ifndef PROJECT2018_STATS_H
#define PROJECT2018_STATS_H


#include <ostream>

class Stats {
private:
    double ratio;
    double time;
    int total;
public:
    Stats():ratio(0.0), time(0.0), total(0){};
    void update(double distance, double realDistance, double time);
    double getMeanTime() const;
    void reset();

    friend std::ostream &operator<<(std::ostream &os, const Stats &stats);
    std::string toString();
};


#endif //PROJECT2018_STATS_H
