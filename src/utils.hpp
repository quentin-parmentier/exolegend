#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <cmath>
#include "MyPosition.hpp"

#undef abs

float moduloPi(float a);
bool checkIfIsOutside(int x, int y, int originalMazeHeight, int originalMazeLength, int *mazeHeight, int *mazeLength);
MyPosition getNearestValidPosition(int robotX, int robotY, int originalMazeHeight, int originalMazeLength, int nextMazeHeight, int nextMazeLength);
float distance(MyPosition pos1, MyPosition pos2);
#endif