#include <vector>
#include <string>
#include "input.h"
#include <cmath>
#include <algorithm>
#include <tuple>
#include <iostream>
#include <map>

struct Junction
{
    int x, y, z;
    size_t parentIndex;

    double distance(const Junction &other) const
    {
        return std::sqrt(
            std::pow(other.x - x, 2) +
            std::pow(other.y - y, 2) +
            std::pow(other.z - z, 2));
    }
};

// Helper function to find root with path compression
size_t findRootIndex(std::vector<Junction> &junctions, size_t index)
{
    if (junctions[index].parentIndex == index)
        return index;
    // Path compression: make every node point directly to root
    junctions[index].parentIndex = findRootIndex(junctions, junctions[index].parentIndex);
    return junctions[index].parentIndex;
}

void connect(int maxConnections = INT_MAX)
{
    const std::vector<std::string> lines = util::readLines("src/day8/input.txt");
    std::vector<Junction> junctions;

    // Start off placing every junction in its own 1 item group
    for (size_t i = 0; i < lines.size(); ++i)
    {
        const std::string &line = lines[i];
        auto coords = util::splitString(line, ",");
        const Junction junction = {
            std::stoi(std::string(coords[0])),
            std::stoi(std::string(coords[1])),
            std::stoi(std::string(coords[2])),
            i // Each junction starts as its own parent
        };
        junctions.emplace_back(junction);
    }

    // Create a sorted list of all junction pairs by distance
    const size_t n = junctions.size();
    std::vector<std::tuple<size_t, size_t, double>> distanceSortedJunctionPairs;

    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = i + 1; j < n; ++j)
        {
            double dist = junctions[i].distance(junctions[j]);
            distanceSortedJunctionPairs.emplace_back(i, j, dist);
        }
    }

    std::sort(distanceSortedJunctionPairs.begin(), distanceSortedJunctionPairs.end(),
              [](const auto &a, const auto &b)
              { return std::get<2>(a) < std::get<2>(b); });

    // Keep track of last connection made
    std::pair<size_t, size_t> lastConnection;

    // Union the closest pairs
    for (int i = 0; i < std::min(maxConnections, (int)distanceSortedJunctionPairs.size()); i++)
    {
        auto pair = distanceSortedJunctionPairs[i];
        size_t aIndex = std::get<0>(pair);
        size_t bIndex = std::get<1>(pair);

        size_t aRootIndex = findRootIndex(junctions, aIndex);
        size_t bRootIndex = findRootIndex(junctions, bIndex);

        // Merge by attaching rootA to rootB
        if (aRootIndex != bRootIndex)
        {
            junctions[aRootIndex].parentIndex = bRootIndex;
            lastConnection = std::make_pair(aIndex, bIndex);
        }
    }

    // Compute set sizes
    std::map<size_t, int> setSizes;
    for (size_t i = 0; i < junctions.size(); ++i)
    {
        size_t rootIndex = findRootIndex(junctions, i);
        setSizes[rootIndex]++;
    }

    std::vector<int> sizes;
    for (const auto &[rootIndex, size] : setSizes)
    {
        sizes.push_back(size);
    }
    std::sort(sizes.begin(), sizes.end(), std::greater<int>());

    size_t result = sizes.size() > 2 ? sizes[0] * sizes[1] * sizes[2] : 0;

    std::cout << "Factor of largest 3 groups: " << result << std::endl;

    Junction &lastJunctionA = junctions[lastConnection.first];
    Junction &lastJunctionB = junctions[lastConnection.second];
    uint64_t lastTwoXMult = (uint64_t)lastJunctionA.x * (uint64_t)lastJunctionB.x;

    std::cout << "Factor of last connected X coords:" << lastTwoXMult << std::endl;
}

int main()
{
    // Part one
    connect(1000);

    // Part two
    connect();
    return 0;
}