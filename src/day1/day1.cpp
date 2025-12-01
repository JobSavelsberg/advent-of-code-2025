#include <iostream>
#include <vector>
#include <string>
#include "../util/input.h"

void partOne()
{
    std::vector<std::string> lines = util::readLines("src/day1/input.txt");

    // Each line is an L or R followed by a distance (integer)
    // E.G. L10 R99
    std::vector<std::pair<char, int>> instructions;
    for (const auto &line : lines)
    {
        instructions.push_back(std::make_pair(line.at(0), std::stoi(line.substr(1))));
    }

    // Loop through the instructions, keeping track of where the dial is pointing
    int dial = 50;
    int dialAt0Count = 0;
    for (const auto &instruction : instructions)
    {
        switch (instruction.first)
        {
        case 'L':
            dial -= instruction.second;
            break;
        case 'R':
            dial += instruction.second;
            break;
        default:
            throw std::runtime_error("Invalid instruction");
        }

        // Make sure dial is always between 0 and 100
        dial = (dial + 100) % 100;

        // Increment count if it is at 0
        if (dial == 0)
        {
            dialAt0Count++;
        }
    }

    std::cout << dialAt0Count << std::endl;
}

void partTwo()
{
    std::vector<std::string> lines = util::readLines("src/day1/input.txt");

    // Each line is an L or R followed by a distance (integer)
    // E.G. L10 R99
    std::vector<std::pair<char, int>> instructions;
    for (const auto &line : lines)
    {
        instructions.push_back(std::make_pair(line.at(0), std::stoi(line.substr(1))));
    }

    // Loop through the instructions, keeping track of where the dial is pointing
    int dial = 50;
    int dialAt0Count = 0;

    for (const auto &instruction : instructions)
    {
        // We can just check where the dial should move to, and how many times it does an extra full rotation
        int fullRotationCount = std::abs(instruction.second / 100);
        int direction = instruction.first == 'L' ? -1 : 1;
        int offset = (instruction.second * direction) % 100;

        // Check if we move INTO/OVER 0 this turn
        int hits0 = dial != 0 && ((dial + offset) >= 100 || (dial + offset) <= 0);

        // Add number of extra rotations (1 extra move over 0 per full rotation)
        dialAt0Count += hits0 + fullRotationCount;

        // Actually move the dial with wrapping
        dial = (dial + offset + 100) % 100;
    }

    std::cout << dialAt0Count << std::endl;
}

int main()
{
    partTwo();
    return 0;
}
