//
// Created by chris on 10/28/2018.
//

#include "FileWriter.h"

FileWriter::FileWriter(std::string filename) {
    myfile = new std::ofstream(filename);

}

FileWriter::~FileWriter() {
    myfile->close();
    delete myfile;
}

void FileWriter::writeLine(std::string line) {
    if(myfile->is_open()){
        *myfile<<line;
    }
}

void FileWriter::close() {
    myfile->close();
}

void FileWriter::writeLine(double val) {
    if(myfile->is_open()){
        *myfile<<val;
    }
}
