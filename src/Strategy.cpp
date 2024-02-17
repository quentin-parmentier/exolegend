#include "Strategy.hpp"
#include "utils.hpp"


Strategy::Strategy(Gladiator *gladiator) : gladiator(gladiator) {
}


void Strategy::generatePath(int pointToWin, MazeSquare *departureMS, MazeSquare *currentMS, MazeSquareList *path, Direction preferedDirection){
    if ((pointToWin <=0))
    {
        return;
    }

    if (preferedDirection == Direction::TOP && currentMS->northSquare != NULL && currentMS->northSquare != departureMS) {
        path->push(currentMS->northSquare);
        generatePath(currentMS->northSquare->possession != 0? pointToWin:pointToWin-1, currentMS, currentMS->northSquare, path, preferedDirection);
        path->push(currentMS);
    } else if (preferedDirection == Direction::RIGHT && currentMS->eastSquare != NULL && currentMS->eastSquare != departureMS) {
        path->push(currentMS->eastSquare);
        generatePath(currentMS->eastSquare->possession != 0? pointToWin:pointToWin-1, currentMS, currentMS->eastSquare, path, preferedDirection);
        path->push(currentMS);
    } else if (preferedDirection == Direction::BOTTOM && currentMS->southSquare != NULL && currentMS->southSquare != departureMS) {
        path->push(currentMS->southSquare);
        generatePath(currentMS->southSquare->possession != 0? pointToWin:pointToWin-1, currentMS, currentMS->southSquare, path, preferedDirection);
        path->push(currentMS);
    } else if (preferedDirection == Direction::LEFT && currentMS->westSquare != NULL && currentMS->westSquare != departureMS) {
        path->push(currentMS->westSquare);
        generatePath(currentMS->westSquare->possession != 0? pointToWin:pointToWin-1, currentMS, currentMS->westSquare, path, preferedDirection);
        path->push(currentMS);
    }

    // NORD
    else if(currentMS->northSquare != NULL && currentMS->northSquare != departureMS){
        path->push(currentMS->northSquare);
        generatePath(currentMS->northSquare->possession != 0? pointToWin:pointToWin-1, currentMS, currentMS->northSquare, path, preferedDirection);
        path->push(currentMS);
    }
    // EST
    else if(currentMS->eastSquare != NULL && currentMS->eastSquare != departureMS){
        path->push(currentMS->eastSquare);
        generatePath(currentMS->eastSquare->possession != 0? pointToWin:pointToWin-1, currentMS, currentMS->eastSquare, path, preferedDirection);
        path->push(currentMS);
    }
    // SUD
    else if(currentMS->southSquare != NULL && currentMS->southSquare != departureMS){
        path->push(currentMS->southSquare);
        generatePath(currentMS->southSquare->possession != 0? pointToWin:pointToWin-1, currentMS, currentMS->southSquare, path, preferedDirection);
        path->push(currentMS);
    }
    // OUEST
    else if(currentMS->westSquare != NULL && currentMS->westSquare != departureMS){
        path->push(currentMS->westSquare);
        generatePath(currentMS->westSquare->possession != 0? pointToWin:pointToWin-1, currentMS, currentMS->westSquare, path, preferedDirection);
        path->push(currentMS);
    }
}

void Strategy::backOnMaze(MazeSquare *currentMS, GameState *gamestate, MazeSquareList *path )
{
    if (currentMS->i < (MAZE_LENGTH - gamestate->actual_maze_length - 1) / 2)
    {
        path->push(currentMS->eastSquare);
    }else if (currentMS->i >= gamestate->actual_maze_length-1)
    {
        path->push(currentMS->westSquare);
    }else if (currentMS->j < (MAZE_HEIGHT - gamestate->actual_maze_height - 1) / 2)
    {
        path->push(currentMS->northSquare);
    }else if(currentMS->j >= gamestate->actual_maze_height-1){
        path->push(currentMS->southSquare);
    }
}