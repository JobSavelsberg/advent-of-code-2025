#include "input.h"
#include <fstream>
#include <iostream>

namespace util
{
    std::vector<std::string> readLines(const std::string &filename)
    {
        std::string inputPath = std::string(SOURCE_DIR) + "/" + filename;
        std::ifstream inputFile(inputPath);

        if (!inputFile.is_open())
        {
            std::cerr << "Error: Could not open " << filename << std::endl;
            return {};
        }

        std::vector<std::string> lines;
        std::string line;

        while (std::getline(inputFile, line))
        {
            lines.push_back(line);
        }

        inputFile.close();
        return lines;
    }
}
