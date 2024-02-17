#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <cmath>
#include "gladiator.h"
#undef abs

float moduloPi(float a);
MazeSquare* msTransfo(const MazeSquare *currentCase);

enum Direction
{
    LEFT = 0,
    RIGHT,
    TOP,
    BOTTOM,
};

#endif