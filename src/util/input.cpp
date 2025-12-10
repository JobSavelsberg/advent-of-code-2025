#include "input.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace util
{
    std::ifstream openFile(const std::string &filename)
    {
        std::string inputPath = std::string(SOURCE_DIR) + "/" + filename;
        std::ifstream inputFile(inputPath);

        if (!inputFile.is_open())
        {
            throw std::runtime_error("Could not open file");
            return {};
        }

        return inputFile;
    }

    std::vector<std::string> readLines(const std::string &filename)
    {
        std::ifstream inputFile = openFile(filename);

        std::vector<std::string> lines;
        std::string line;

        while (std::getline(inputFile, line))
        {
            lines.push_back(line);
        }

        inputFile.close();
        return lines;
    }

    std::vector<std::string> readCommaSeparated(const std::string &filename)
    {
        std::ifstream inputFile = openFile(filename);
        std::vector<std::string> items;
        std::string item;
        while (std::getline(inputFile, item, ','))
        {
            items.push_back(item);
        }

        inputFile.close();
        return items;
    }

    std::vector<std::string_view> splitString(const std::string_view input, const std::string_view delimiters)
    {
        std::vector<std::string_view> result;
        std::size_t last_pos = 0;
        for (std::size_t i = 0; i < input.size(); ++i)
        {
            if (delimiters.find(input[i]) != delimiters.npos)
            {
                result.push_back(input.substr(last_pos, i - last_pos));
                ++i;
                last_pos = i;
            }
        }

        result.push_back(input.substr(last_pos, input.size()));
        return result;
    }
}
