//
// Created by chris on 10/28/2018.
//

#ifndef PROJECT2018_FILEWRITER_H
#define PROJECT2018_FILEWRITER_H


#include <fstream>

class FileWriter {
private:
    std::ofstream *myfile;

public:
    explicit FileWriter(std::string filename);

    virtual ~FileWriter();

    void writeLine(std::string line);

    void writeLine(double val);


    void close();
};


#endif //PROJECT2018_FILEWRITER_H
