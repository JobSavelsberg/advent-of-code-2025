#include <fstream>
#include <input.h>
#include <iostream>
#include <numeric>

using Number = long long;

std::vector<size_t> findAll(const std::string &s, char c)
{
    std::vector<size_t> result;
    for (size_t i = 0; i < s.size(); ++i)
    {
        if (s[i] == c)
        {
            result.push_back(i);
        }
    }
    return result;
}

void partOne()
{
    std::ifstream inputFile = util::openFile("src/day7/input.txt");
    std::string line;

    std::vector<bool> beams;

    // While reading each line, keep track of the beams that go down as we sweep.
    size_t lineIndex = 0;
    int beamSplitCount = 0;
    while (std::getline(inputFile, line))
    {
        // Set up when we get the first line
        if (lineIndex == 0)
        {
            beams.resize(line.length());
            beams[line.find("S")] = true;
        }

        if (lineIndex > 0)
        {
            // For all splitters, split f a beam is there
            for (const size_t i : findAll(line, '^'))
            {
                // There is a beam to be split
                if (beams[i])
                {
                    int paths = 0;
                    // Split to left if possible
                    if (i - 1 > 0 && line[i - 1] == '.')
                    {
                        beams[i - 1] = true;
                        paths++;
                    }

                    // Split to right if possible
                    if (i + 1 <= beams.size() - 1 && line[i + 1] == '.')
                    {
                        beams[i + 1] = true;
                        paths++;
                    }

                    // Beam cannot go straight anymore
                    beams[i] = false;
                    beamSplitCount++;
                }
            }
        }

        lineIndex++;
    }

    std::cout << beamSplitCount << std::endl;
}

void partTwo()
{
    std::ifstream inputFile = util::openFile("src/day7/input.txt");
    std::string line;

    std::vector<Number> beams;

    // While reading each line, keep track of the beams that go down as we sweep.
    // Since dealing with "timelines", we can keep track of the number of beams/timelines in one spot instead
    size_t lineIndex = 0;
    while (std::getline(inputFile, line))
    {
        // Set up when we get the first line
        if (lineIndex == 0)
        {
            beams.resize(line.length());
            beams[line.find("S")] = true;
        }

        if (lineIndex > 0)
        {
            // For all splitters, split f a beam is there
            for (const size_t i : findAll(line, '^'))
            {
                // There is a beam to be split
                if (beams[i])
                {
                    // Split to left if possible
                    if (i - 1 > 0 && line[i - 1] == '.')
                    {
                        beams[i - 1] += beams[i];
                    }

                    // Split to right if possible
                    if (i + 1 <= beams.size() - 1 && line[i + 1] == '.')
                    {
                        beams[i + 1] += beams[i];
                    }

                    // Beam cannot go straight anymore
                    beams[i] = 0;
                }
            }
        }

        lineIndex++;
    }

    Number timeLineCount = std::accumulate(beams.begin(), beams.end(), (Number)0) + 1;

    std::cout << timeLineCount << std::endl;
}

int main()
{
    partTwo();
    return 0;
}