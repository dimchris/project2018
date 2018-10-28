//
// Created by chris on 10/28/2018.
//

#ifndef PROJECT2018_RECORDWRITER_H
#define PROJECT2018_RECORDWRITER_H


#include "FileWriter.h"
#include "../Model/MyQuery.h"
#include "../UtilClasses/Compare.h"
#include "../UtilClasses/Stats.h"
#include <set>
#include <list>
#include <sstream>

template<class T>
class RecordWriter : protected FileWriter {
public:
    RecordWriter(std::string filename) : FileWriter(filename) { stats = new Stats(); }

    ~RecordWriter() { delete stats; }

    void writeRecord(MyQuery<T> *query, std::set<Neighbor<T> *, Compare<T> > neighbors, Neighbor<int> *trueNeighbor,
                     double timeLSH, double timeTrue);

    void writeStats();

private:
    Stats *stats;

};

template<class T>
void RecordWriter<T>::writeRecord(MyQuery<T> *query, std::set<Neighbor<T> *, Compare<T>> neighbors,
                                  Neighbor<int> *trueNeighbor, double timeLSH, double timeTrue) {
    FileWriter::writeLine("Query:");
    FileWriter::writeLine(query->getVector()->getId());
    FileWriter::writeLine("\n");
    FileWriter::writeLine("R-near neighbors:\n");
    typename std::set<Neighbor<T> *>::const_iterator iterator;
    for (iterator = neighbors.begin(); iterator != neighbors.end(); ++iterator) {
        FileWriter::writeLine((*iterator)->getVector()->getId());
        FileWriter::writeLine("\n");
    }
    iterator = neighbors.begin();
    FileWriter::writeLine("Nearest neighbor: ");
    if (neighbors.size() > 0)
        FileWriter::writeLine((*iterator)->getVector()->getId());
    FileWriter::writeLine("\n");
    FileWriter::writeLine("distanceLSH: ");
    if (neighbors.size() > 0)
        FileWriter::writeLine((*iterator)->getDistance());
    FileWriter::writeLine("\n");
    FileWriter::writeLine("distanceTrue: ");
    FileWriter::writeLine(trueNeighbor->getDistance());
    FileWriter::writeLine("\n");
    FileWriter::writeLine("tLSH: ");
    FileWriter::writeLine(timeLSH);
    FileWriter::writeLine("\n");
    FileWriter::writeLine("tTrue: ");
    FileWriter::writeLine(timeTrue);
    FileWriter::writeLine("\n");
    //update stats
    if (neighbors.size() > 0)
        stats->update((*iterator)->getDistance(), trueNeighbor->getDistance(), timeLSH);
}

template<class T>
void RecordWriter<T>::writeStats() {
    std::ostringstream strs;
    strs << *stats;
    FileWriter::writeLine(strs.str());
}


#endif //PROJECT2018_RECORDWRITER_H
