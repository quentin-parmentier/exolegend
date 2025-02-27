#ifndef NAVIGATIONSTRATEGY_HPP_
#define NAVIGATIONSTRATEGY_HPP_

#include "NavigationStack.hpp"
#include "MyPosition.hpp"
#include "gladiator.h"
#include <vector>
#include <set>
#include <algorithm>

enum Direction
{
    LEFT,
    RIGHT,
    TOP,
    BOTTOM,
};

class NavigationStrategy
{

public:
    NavigationStrategy(NavigationStack *navigationStack, Gladiator *gladiator, int depthWalking, const MazeSquare ***maze, int *mazeHeight, int *mazeLength, int originalMazeHeight, int originalMazeLength);
    void computeBestPath(MyPosition actualPos, std::vector<MyPosition> currentPath, std::vector<MyPosition>& bestPath, int currentScore, int& maxScore);    
    bool isOnMazeBorder(int x, int y);
    bool isOutside(int x, int y);
    int valueOfMS(const MazeSquare *ms, const bool throughWall, std::vector<MyPosition>& visited);
    int costOfMS(MazeSquare ms);
    Direction getReverseDirection(Direction direction);
    Direction getRandomDirection(Direction lastDirection, bool tryToGoForward);
    MyPosition getNextCase(Direction direction, MyPosition position);
    bool goingThroughWall(Direction direction, const MazeSquare *mazeSquare);

private:
    NavigationStack *navigationStack;
    Gladiator *gladiator;
    int depthWalking;
    const MazeSquare ***maze;
    int *mazeHeight;
    int *mazeLength;
    int originalMazeHeight;
    int originalMazeLength;
};

#endif