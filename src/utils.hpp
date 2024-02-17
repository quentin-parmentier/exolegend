#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <cmath>
#undef abs

float moduloPi(float a);
bool checkIfIsOutside(int x, int y, int originalMazeHeight, int originalMazeLength, int *mazeHeight, int *mazeLength);

#endif