#ifndef STRATEGY_HPP_
#define STRATEGY_HPP_

#include <gladiator.h>
#include "MazeSquareList.hpp"
#include "GameData.hpp"
#include "utils.hpp"

class Strategy
{
public:
    Strategy(Gladiator *gladiator);
    void backOnMaze(MazeSquare *currentMS, GameState *gamestate, MazeSquareList *path );
    void generatePath(int pointToWin, MazeSquare *departureMS, MazeSquare *currentMS, MazeSquareList *path, Direction preferedDirection);
    
private:
    Gladiator *gladiator;
};
#endif