#include <iostream>
#include <vector>
#include <string>
#include "input.h"
#include <algorithm>

using Number = long long;

// I just didn't like to use std::pair because of 'first' and 'second'.
struct Range
{
    Number start;
    Number end;

    bool operator<(const Range &other) const
    {
        // Sorting on end is not needed for this puzzle, but I would go slightly mad if it would not sort lexicographically
        if (start == other.start)
        {
            return end < other.end;
        }
        return start < other.start;
    }
};

struct Database
{
    std::vector<Range> ranges;
    std::vector<Number> ids;
};

// Extract and format data O(n)
Database getDatabase(const std::string &fileName)
{
    std::vector<std::string> lines = util::readLines(fileName);

    Database db;

    for (const std::string &line : lines)
    {
        size_t dashIndex = line.find('-');
        if (dashIndex != std::string::npos)
        {
            Number start = std::stoll(line.substr(0, dashIndex));
            Number end = std::stoll(line.substr(dashIndex + 1));
            db.ranges.emplace_back(Range{start, end});
        }
        else if (!line.empty())
        {
            db.ids.push_back(std::stoll(line));
        }
    }

    return db;
}

void partOne()
{
    Database db = getDatabase("src/day5/input.txt");

    // Sort O(n*log(n))
    std::sort(db.ranges.begin(), db.ranges.end());
    std::sort(db.ids.begin(), db.ids.end());

    // Match ids with ranges O(n)
    int freshCount = 0;

    int idIndex = 0;
    int rangeIndex = 0;
    while (idIndex < db.ids.size() && rangeIndex < db.ranges.size())
    {
        Number id = db.ids[idIndex];
        Number rangeStart = db.ranges[rangeIndex].start;
        Number rangeEnd = db.ranges[rangeIndex].end;

        // If id is smaller, go to next id
        if (id < rangeStart)
        {
            idIndex++;
            continue;
        }

        // If id is in range, it is fresh, move to next id
        if (id >= rangeStart && id <= rangeEnd)
        {
            freshCount++;
            idIndex++;
            continue;
        }

        // If id is larger, go to next range
        if (id > rangeEnd)
        {
            rangeIndex++;
            continue;
        }
    }

    std::cout
        << freshCount << std::endl;
}

void partTwo()
{
    Database db = getDatabase("src/day5/input.txt");

    // Sort O(n*log(n))
    std::sort(db.ranges.begin(), db.ranges.end());

    std::vector<Range> mergedRanges;

    // Do a pass to merge overlapping ranges O(n)
    Number mergingRangeStart = db.ranges[0].start;
    Number mergingRangeEnd = db.ranges[0].end;
    for (const Range &range : db.ranges)
    {
        // Merge if the start of the range is in the range we're merging
        if (range.start >= mergingRangeStart && range.start <= mergingRangeEnd)
        {
            mergingRangeEnd = std::max(range.end, mergingRangeEnd);
            continue;
        }

        // If the start of the range is outside the range we're merging, we need to create a new merged range
        if (range.start > mergingRangeEnd)
        {
            // Save the merged range we made up to now
            mergedRanges.emplace_back(Range{mergingRangeStart, mergingRangeEnd});

            // Start merging a new range
            mergingRangeStart = range.start;
            mergingRangeEnd = range.end;
        }
    }

    // Add the final range
    mergedRanges.emplace_back(Range{mergingRangeStart, mergingRangeEnd});

    // Count the total size of all merged ranges O(n)
    Number totalRangesSize = 0;
    for (const Range &mergedRange : mergedRanges)
    {
        totalRangesSize += mergedRange.end - mergedRange.start + 1;
    }

    std::cout
        << totalRangesSize << std::endl;
}

int main()
{
    partTwo();
    return 0;
}
