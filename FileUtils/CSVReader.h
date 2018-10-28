//
// Created by chris on 10/24/2018.
//

#ifndef PROJECT2018_CSVREADER_H
#define PROJECT2018_CSVREADER_H


#include <vector>
#include "FileReader.h"
#include "../UtilClasses/Utils.h"

class CSVReader : protected FileReader {
private:
    bool hasHeader;
    char delimiter;

protected:
    std::string *header;
public:
    CSVReader(std::string filename, char delimiter, bool hasHeader) : FileReader(filename), delimiter(delimiter),
                                                                      hasHeader(hasHeader), header(NULL) {
        if(hasHeader){
            header = FileReader::readLine();
            Utils::trim(*header);
        }
    };

    ~CSVReader() {
        if(hasHeader){
            delete header;
        }

    }

    std::vector<std::string> *getRecord();

    void close();
};


#endif //PROJECT2018_CSVREADER_H
