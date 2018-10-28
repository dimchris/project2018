//
// Created by chris on 10/24/2018.
//

#ifndef PROJECT2018_FILEREADER_H
#define PROJECT2018_FILEREADER_H

#include <iostream>
#include <fstream>

class FileReader {
private:
    std::ifstream *myfile;

public:
    explicit FileReader(std::string filename);

    virtual ~FileReader();

    std::string * readLine();

    void close();

};


#endif //PROJECT2018_FILEREADER_H
