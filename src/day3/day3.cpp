#include <iostream>
#include "input.h"

std::vector<std::vector<int>> createGridOfJoltages(const std::string &inputFile)
{
    const std::vector<std::string> lines = util::readLines(inputFile);

    // Turn each line in a vector of ints
    std::vector<std::vector<int>> grid;

    for (const std::string line : lines)
    {
        std::vector<int> bank;
        for (const char digitChar : line)
        {
            bank.emplace_back(std::stoi(std::string(1, digitChar)));
        }
        grid.emplace_back(bank);
    }

    return grid;
}

void partOne()
{
    const std::vector<std::vector<int>> grid = createGridOfJoltages("src/day3/input.txt");

    int totalJoltage = 0;

    for (const std::vector<int> bank : grid)
    {
        int highestFirstDigit = 0;
        int highestSecondDigit = 0;
        for (int i = 0; i < bank.size(); i++)
        {
            int joltage = bank[i];

            // Only overwrite first digit when there is actually a second one after
            if (joltage > highestFirstDigit && i < bank.size() - 1)
            {
                highestFirstDigit = joltage;
                // We need to start looking from scratch again for the second highest
                highestSecondDigit = 0;
            }
            else
            {
                if (joltage > highestSecondDigit)
                {
                    highestSecondDigit = joltage;
                }
            }
        }

        int bankJoltage = highestFirstDigit * 10 + highestSecondDigit;
        totalJoltage += bankJoltage;
    }

    std::cout << totalJoltage << std::endl;
}

void partTwo()
{
    // Idea is to start with twelve digits on the right selected
    // Move the leftmost selected digit to the left to find the highest number
    // Then move the second to leftmost selected digit to the left (up to where first digit is) to find the highest number
    // Etc. until we tried to move all 12 selected digits

    const std::vector<std::vector<int>> grid = createGridOfJoltages("src/day3/input.txt");

    const int bankSize = grid[0].size();
    const int numberOfBatteries = 12;

    long long totalJoltage = 0;

    for (const std::vector<int> bank : grid)
    {
        // Select last 12 batteries (by index, so 5 means battery 5, which might have joltage 1)
        std::vector<int> selectedBatteries;
        for (int b = bankSize - numberOfBatteries; b < bankSize; b++)
        {
            selectedBatteries.emplace_back(b);
        }

        // Now for each selected battery, see what the highest battery joltage to the left is
        for (int i = 0; i < selectedBatteries.size(); i++)
        {
            // The battery index that we're looking at
            const int b = selectedBatteries[i];

            int lowestPossibleBattery = 0;

            // Don't let the battery cross the previous one
            // Not relevant for first selected index, hence the if
            if (i > 0)
            {
                // E.g. batteryBank = [2,6,4,5,9,8,7,6], selectedBatteries = [2,5,6,7];
                // When we want to see what we can select for the second selected battery (now pointing to battery nr 5)
                // We can only move down to the previous selected battery 2 plus 1 to the right, we cannot select the same battery twice
                lowestPossibleBattery = selectedBatteries[i - 1] + 1;
            }

            int highestJoltage = bank[b];
            // Start looking from the battery to the left of it, until the leftmost possible battery
            for (int j = b - 1; j >= lowestPossibleBattery; j--)
            {
                // We are looking from right to left,
                // so we make sure, if they are the same, we pick the leftmost one
                // by using >= and not >
                if (bank[j] >= highestJoltage)
                {
                    selectedBatteries[i] = j;
                    highestJoltage = bank[j];
                }
            }
        }

        // Get the number for the selected indexes
        long long bankJoltage = 0;
        for (int i = 0; i < selectedBatteries.size(); i++)
        {
            // E.g. when we have 7 batteries, the first battery is the 'million' digit.
            // So order would be (7 - 0 - 1) = 6
            // 6 * 10 = 1.000.000
            // Then we multiply a million with the joltage of that battery
            const int order = numberOfBatteries - i - 1;
            const long long multiplier = static_cast<long long>(std::pow(10, order));
            const long long joltage = bank[selectedBatteries[i]] * multiplier;
            bankJoltage += joltage;
        }
        totalJoltage += bankJoltage;
    }

    std::cout << totalJoltage << std::endl;
}

int main()
{
    partTwo();
    return 0;
}
