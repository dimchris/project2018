//
// Created by chris on 10/24/2018.
//

#ifndef PROJECT2018_UTILS_H
#define PROJECT2018_UTILS_H


#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <locale>

class Utils {
public:
    static std::vector<std::string> split(const std::string &s, char delim);
    // trim from start (in place)
    static  void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
            return !std::isspace(ch);
        }));
    }

// trim from end (in place)
    static  void rtrim(std::string &s);

// trim from both ends (in place)
    static  void trim(std::string &s);

// trim from start (copying)
    static  std::string ltrim_copy(std::string s);

// trim from end (copying)
    static  std::string rtrim_copy(std::string s);
};


#endif //PROJECT2018_UTILS_H
