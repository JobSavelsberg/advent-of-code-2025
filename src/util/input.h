#ifndef INPUT_H
#define INPUT_H

#include <vector>
#include <string>

namespace util
{
    std::ifstream openFile(const std::string &filename);
    std::vector<std::string> readLines(const std::string &filename);
    std::vector<std::string> readCommaSeparated(const std::string &filename);
    std::vector<std::string_view> splitString(const std::string_view input, const std::string_view delimiters);
}

#endif
