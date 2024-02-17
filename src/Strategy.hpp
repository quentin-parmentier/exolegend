#ifndef STRATEGY_HPP_
#define STRATEGY_HPP_

#include <gladiator.h>
#include "MazeSquareList.hpp"
#include "GameData.hpp"

class Strategy
{
public:
    Strategy(Gladiator *gladiator);
    void generatePath(int pointToWin, MazeSquare *departureMS, MazeSquare *currentMS, MazeSquareList *path);
    void backOnMaze(MazeSquare *currentMS, GameState *gamestate, MazeSquareList *path );
    
private:
    Gladiator *gladiator;
};
#endif