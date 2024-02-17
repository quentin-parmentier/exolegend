#include "utils.hpp"

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