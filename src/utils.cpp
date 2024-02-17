#include "utils.hpp"
#include <iostream>

float moduloPi(float a) // return angle in [-pi; pi]
{
    return (a < 0.0) ? (std::fmod(a - M_PI, 2 * M_PI) + M_PI) : (std::fmod(a + M_PI, 2 * M_PI) - M_PI);
}

bool checkIfIsOutside(int x, int y, int originalMazeHeight, int originalMazeLength, int *mazeHeight, int *mazeLength)
{

    if (x < (originalMazeLength - (*mazeLength)) / 2 || x >= (*mazeLength) || y < (originalMazeHeight - (*mazeHeight)) / 2 || y >= (*mazeHeight))
    {
        return true;
    }

    return false;
}

MyPosition getNearestValidPosition(int robotX, int robotY, int originalMazeHeight, int originalMazeLength, int nextMazeHeight, int nextMazeLength)
{
    MyPosition nearestPosition = MyPosition();
    double distanceMin = std::numeric_limits<double>::max();

    /// On parcours tous les bords du terrain
    int minX = (originalMazeLength - nextMazeLength) / 2;
    int minY = (originalMazeHeight - nextMazeHeight) / 2;
    int maxX = nextMazeLength - 1;
    int maxY = nextMazeHeight - 1;

    MyPosition robotPos = MyPosition(robotX, robotY);

    for (int x = minX; x <= maxX; x++)
    {
        for (int y = minY; y <= maxY; y++)
        {
            MyPosition testedPos = MyPosition(x, y);
            double distanceForPos = distance(robotPos, testedPos);
            if (distanceForPos < distanceMin)
            {
                distanceMin = distanceForPos;
                nearestPosition = testedPos;
            }
        }
    }

    return nearestPosition;
}

float distance(MyPosition pos1, MyPosition pos2)
{
    return std::sqrt(std::pow(pos1.getX() - pos2.getX(), 2) + std::pow(pos1.getY() - pos2.getY(), 2));
}