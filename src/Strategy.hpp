#ifndef STRATEGY_HPP_
#define STRATEGY_HPP_

#include <gladiator.h>
#include "MazeSquareList.hpp"

class Strategy
{
public:
    Strategy(Gladiator *gladiator);
    void generatePath(int pointToWin, MazeSquare *departureMS, MazeSquare *currentMS, MazeSquareList *path);
    
private:
    Gladiator *gladiator;
};
#endif