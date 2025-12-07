#include <vector>
#include <string>
#include "input.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using Number = long long;

void partOne()
{
    std::ifstream inputFile = util::openFile("src/day6/input.txt");
    std::string line;

    std::vector<std::vector<std::string>> cols;

    while (std::getline(inputFile, line))
    {
        std::istringstream iss(line);
        std::string value;

        size_t colIndex = 0;
        while (iss >> value)
        {
            // Add a new column if needed
            if (colIndex >= cols.size())
                cols.emplace_back();

            cols[colIndex].push_back(value);
            colIndex++;
        }
    }

    Number sum = 0;

    for (const std::vector<std::string> col : cols)
    {
        auto operation = col.at(col.size() - 1);
        Number result = std::stoll(col[0]);

        for (int i = 1; i < col.size() - 1; i++)
        {
            if (operation == "*")
            {
                result *= std::stoll(col[i]);
            }
            else if (operation == "+")
            {
                result += std::stoll(col[i]);
            }
        }

        sum += result;
    }

    std::cout << sum << std::endl;
}

bool isWhitespaceOnly(const std::string &s)
{
    return std::all_of(s.begin(), s.end(), [](unsigned char c)
                       { return std::isspace(c); });
}

void partTwo()
{
    std::ifstream inputFile = util::openFile("src/day6/input.txt");
    std::string line;

    std::vector<std::vector<std::string>> rows;

    // Split lines into 1 character strings
    while (std::getline(inputFile, line))
    {
        std::vector<std::string> row;
        for (char c : line)
        {
            row.push_back(std::string(1, c));
        }
        rows.push_back(row);
    }

    // Transform 90 degrees
    std::vector<std::string> cols(rows[0].size());
    for (size_t i = 0; i < rows.size(); i++)
    {
        for (size_t j = 0; j < rows[i].size(); j++)
        {
            cols[j] += rows[i][j];
        }
    }

    Number subComputationTotal = 0;
    Number totaltotal = 0;
    std::string operation = "";
    for (size_t i = 0; i < cols.size(); i++)
    {
        // Add subComputation total and reset
        if (isWhitespaceOnly(cols[i]))
        {
            operation = "";
            totaltotal += subComputationTotal;
            subComputationTotal = 0;
            continue;
        }

        auto numberString = cols[i].substr(0, cols[i].size() - 1);
        auto number = std::stoi(numberString);
        auto newOperation = cols[i].at(cols[i].size() - 1);

        // If it is the first number, it also has the new operation
        if (!std::isspace(newOperation))
        {
            operation = newOperation;
            subComputationTotal = number;
            continue;
        }

        // Perform operation on number
        if (operation == "*")
        {
            subComputationTotal *= number;
        }
        else if (operation == "+")
        {
            subComputationTotal += number;
        }
    }

    totaltotal += subComputationTotal;

    std::cout << totaltotal << std::endl;
}

int main()
{
    partTwo();
    return 0;
}