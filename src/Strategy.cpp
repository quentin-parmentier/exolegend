#include"Strategy.hpp"

Strategy::Strategy(Gladiator *gladiator) : gladiator(gladiator) {
}

void Strategy::generatePath(int pointToWin, MazeSquare *departureMS, MazeSquare *currentMS, MazeSquareList *path){
    if ((pointToWin <=0))
    {
        return;
    }
    // NORD
    else if(currentMS->northSquare != NULL && currentMS->northSquare != departureMS){
        path->push(currentMS->northSquare);
        generatePath(currentMS->northSquare->possession != 0? pointToWin:pointToWin-1, currentMS, currentMS->northSquare, path );
        path->push(currentMS);
    }
    //EST
    else if(currentMS->eastSquare != NULL && currentMS->eastSquare != departureMS){
        path->push(currentMS->eastSquare);
        generatePath(currentMS->eastSquare->possession != 0? pointToWin:pointToWin-1, currentMS, currentMS->eastSquare, path );
        path->push(currentMS);
    }
    // SUD
    else if(currentMS->southSquare != NULL && currentMS->southSquare != departureMS){
        path->push(currentMS->southSquare);
        generatePath(currentMS->southSquare->possession != 0? pointToWin:pointToWin-1, currentMS, currentMS->southSquare, path );
        path->push(currentMS);
    }
    //OUEST
    else if(currentMS->westSquare != NULL && currentMS->westSquare != departureMS){
        path->push(currentMS->westSquare);
        generatePath(currentMS->westSquare->possession != 0? pointToWin:pointToWin-1, currentMS, currentMS->westSquare, path );
        path->push(currentMS);
    }
}