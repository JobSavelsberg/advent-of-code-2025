#include <iostream>
#include <boost/algorithm/string.hpp>
#include <cmath>
#include "input.h"

bool isOdd(int num)
{
    return num % 2 != 0;
}

void partOne()
{
    std::vector<std::string> ranges = util::readCommaSeparated("src/day2/input.txt");

    long repeatedSequenceSum = 0;

    // Unpack "1234-2345" into start and end
    for (const auto &range : ranges)
    {
        std::vector<std::string> parts;
        boost::split(parts, range, boost::is_any_of("-"));

        std::string startString = parts[0];
        std::string endString = parts[1];

        long start = std::stol(startString);
        long end = std::stol(endString);

        int startDigitCount = startString.length();
        int endDigitCount = endString.length();

        // We cannot have a number repeated twice if the possible numbers are always odd
        if (isOdd(startDigitCount) && startDigitCount == endDigitCount)
            continue;

        long repeatedSequenceInRangeSum = 0;

        // Loop through range
        for (long number = start; number <= end; number++)
        {
            std::string numberString = std::to_string(number);
            if (isOdd(numberString.length()))
                continue;

            // Split the numebrString into 2
            int middle = numberString.length() / 2;
            std::string firstHalf = numberString.substr(0, middle);
            std::string secondHalf = numberString.substr(middle);

            if (firstHalf == secondHalf)
            {
                repeatedSequenceInRangeSum += number;
            }
        }

        repeatedSequenceSum += repeatedSequenceInRangeSum;
    }

    std::cout << repeatedSequenceSum << std::endl;
}

bool isNumberRepeatedPatternWithNumDigits(long number, int numDigits)
{
    std::string numberString = std::to_string(number);

    // If number is not divisible by numDigits, it won't work
    if (numberString.length() % numDigits != 0)
        return false;

    // Keep getting last numdigits of number, then divide, and make sure it is the same
    // E.g. 123123, numdigits = 3
    // 123123 % 10^3 = 123
    int divisor = std::pow(10, numDigits);
    long patternNumber = number % divisor;
    long dividedNumber = number;
    while (dividedNumber > 0)
    {
        if (dividedNumber % divisor != patternNumber)
        {
            return false;
        }

        // Since it is an int, it will automatically floor
        // E.g. 123123 / 1000 = float 123.123 = int 123
        dividedNumber /= divisor;
    }

    return true;
}

bool isNumberRepeatedPattern(long number)
{
    std::string numberString = std::to_string(number);

    // Look for repeated digits, start with 1 digit (e.g. 777) then 2 (6767), then 3 (234234234) etc. up to half the number of digits
    for (int numDigits = 1; numDigits <= numberString.length() / 2; numDigits++)
    {
        if (isNumberRepeatedPatternWithNumDigits(number, numDigits))
        {
            return true;
        }
    }

    return false;
}

void partTwo()
{
    std::vector<std::string> ranges = util::readCommaSeparated("src/day2/input.txt");

    long repeatedSequenceSum = 0;

    // Unpack "1234-2345" into start and end
    for (const auto &range : ranges)
    {
        std::vector<std::string> parts;
        boost::split(parts, range, boost::is_any_of("-"));

        std::string startString = parts[0];
        std::string endString = parts[1];

        long start = std::stol(startString);
        long end = std::stol(endString);

        int startDigitCount = startString.length();
        int endDigitCount = endString.length();

        // Loop through range
        for (long number = start; number <= end; number++)
        {
            if (isNumberRepeatedPattern(number))
            {
                repeatedSequenceSum += number;
            }
        }
    }

    std::cout << repeatedSequenceSum << std::endl;
}

int main()
{
    partTwo();
    return 0;
}
