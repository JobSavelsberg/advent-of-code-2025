#include <vector>
#include <string>
#include <input.h>
#include <algorithm>
#include <iostream>

struct Point2D
{
    int x, y;
};

// A rect is valid when it does not have any points or line segments across it (apart from its boundary)
bool isValidRect(const std::vector<Point2D> &points, size_t aIndex, size_t bIndex)
{
    const Point2D &a = points[aIndex];
    const Point2D &b = points[bIndex];

    if (a.x == b.x && a.y == b.y)
        return false;

    int minX = std::min(a.x, b.x);
    int maxX = std::max(a.x, b.x);
    int minY = std::min(a.y, b.y);
    int maxY = std::max(a.y, b.y);

    // Check if any point lies strictly inside the rectangle (not on boundary)
    for (size_t i = 0; i < points.size(); ++i)
    {
        if (i == aIndex || i == bIndex)
            continue;

        const Point2D &p = points[i];
        if (p.x > minX && p.x < maxX && p.y > minY && p.y < maxY)
        {
            return false;
        }
    }

    // Check if any line segment goes across the rect
    for (size_t i = 0; i < points.size(); ++i)
    {
        const Point2D &p1 = points[i];
        const Point2D &p2 = points[(i + 1) % points.size()];

        // Ignore if completely outside
        if ((p1.x <= minX && p2.x <= minX) || p1.x >= maxX && p2.x >= maxX)
            continue;
        if ((p1.y <= minY && p2.y <= minY) || p1.y >= maxY && p2.y >= maxY)
            continue;

        // We are only dealing with horizontal or vertical lines
        // Check horizontal line
        if (p1.y == p2.y && p1.y > minY && p1.y < maxY)
        {
            if (std::min(p1.x, p2.x) <= minX && std::max(p1.x, p2.x) >= maxX)
                return false;
        }
        // Check vertical line
        if (p1.x == p2.x && p1.x > minX && p1.x < maxX)
        {
            if (std::min(p1.y, p2.y) <= minY && std::max(p1.y, p2.y) >= maxY)
                return false;
        }
    }
    return true;
}

void getBiggestArea(bool checkRectValidity = false)
{
    // Turn text file into vector of 2d Points
    const std::vector<std::string> &lines = util::readLines("src/day9/input.txt");
    std::vector<Point2D> points;
    for (size_t i = 0; i < lines.size(); ++i)
    {
        auto coords = util::splitString(lines[i], ",");
        int x = std::stoi(std::string(coords[0]));
        int y = std::stoi(std::string(coords[1]));
        points.push_back(Point2D{x, y});
    }

    // Do an O(n^2) search for biggest area
    long long maxArea = 0;
    for (int a = 0; a < points.size(); a++)
    {
        for (int b = 0; b < points.size(); b++)
        {
            const Point2D &pointA = points[a];
            const Point2D &pointB = points[b];

            long long horizontalLength = std::abs(pointA.x - pointB.x) + 1;
            long long verticalLength = std::abs(pointA.y - pointB.y) + 1;
            long long area = horizontalLength * verticalLength;
            if (area > maxArea)
            {
                if (!checkRectValidity || isValidRect(points, a, b))
                {
                    maxArea = area;
                }
            }
        }
    }

    std::cout << maxArea << std::endl;
}

int main()
{
    // Part 1
    getBiggestArea();
    // Part 2
    getBiggestArea(true);
    return 0;
}