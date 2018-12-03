//
// Created by chris on 11/25/2018.
//

#include "ConfigReader.h"

ConfigReader::ConfigReader(std::string input_file) {
    try{
        FileReader reader = FileReader(input_file);
        std::string *line;
        while(line = reader.readLine()){
            std::vector<std::string> config = Utils::split(*line, ':');
            configuration[config[0]] = config[1];
            delete line;
        }
    }catch (std::runtime_error error){
        throw error;
    }
}

std::string ConfigReader::get(std::string conf) {
    return configuration[conf];
}
