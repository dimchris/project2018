//
// Created by chris on 10/24/2018.
//

#include "FileReader.h"

FileReader::FileReader(std::string filename) {
    myfile = new std::ifstream(filename);
    if(!myfile->good()){
        throw std::runtime_error("could not open file");
    }
}

FileReader::~FileReader() {
    myfile->close();
    delete myfile;
}

std::string * FileReader::readLine() {
    std::string *line = new std::string;
    if(myfile->is_open() && std::getline(*myfile, *line)){
        return line;
    }
    return NULL;
}

void FileReader::close() {
    myfile->close();
}
