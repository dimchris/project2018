//
// Created by chris on 10/24/2018.
//

#include <sstream>
#include "Utils.h"

std::vector<std::string> Utils::split(const std::string &s, char delim) {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

void Utils::rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void Utils::trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

std::string Utils::ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}

std::string Utils::rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}
