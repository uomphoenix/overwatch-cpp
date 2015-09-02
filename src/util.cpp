/*
util.cpp

Includes useful utility functions.


Credit to Evan Teran for string splitting fuctions
    http://stackoverflow.com/a/236803


*/

#include <string>
#include <sstream>
#include <vector>

#include "include/util.h"

namespace util
{

std::vector<std::string> &split(const std::string &s, char delim,
                                std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

}
