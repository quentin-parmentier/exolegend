#include "gladiator.h"
#include "Vector2.hpp"
#include "Navigation.hpp"
#include "utils.hpp"

// #include <random>
#include <algorithm> // Pour std::copy
#include <iostream>
#include "MyPosition.hpp"
#include "Timer.hpp"
#include "GameData.hpp"
#include "Strategy.hpp"
#include "MazeSquareList.hpp"

#undef abs

// const int MAZE_HEIGHT = 12;
// const int MAZE_LENGTH = 12;
// int ACTUAL_MAZE_HEIGHT = MAZE_HEIGHT;
// int ACTUAL_MAZE_LENGTH = MAZE_LENGTH;
// const MazeSquare *maze[MAZE_HEIGHT][MAZE_LENGTH];
const int DEPTH_WALKING = 5;

const MyPosition ROBOT_POSITION = MyPosition(0, 0); /// On peut peut être récupérer ça avec le Gladiator @todo
Gladiator *gladiator;
Navigation *navigation;
GameState *gamestate;
Strategy *strategy;
MazeSquareList *msList;
MazeSquare *caseCible;

void reset()
{
    gamestate->reset();
    navigation->reset();
    caseCible = nullptr;
}

// Pour tester le robot, ne pas oublier de commenter pour les matchs !!!
// #define FREE_MODE

void setup()
{
    // instanciation de l'objet gladiator
    gladiator = new Gladiator();
    navigation = new Navigation(gladiator);
    gamestate = new GameState(gladiator);
    strategy = new Strategy(gladiator);
    msList = new MazeSquareList(100);
    caseCible = nullptr;
    // enregistrement de la fonction de reset qui s'éxecute à chaque fois avant qu'une partie commence
    gladiator->game->onReset(&reset);

#ifdef FREE_MODE
    gladiator->game->enableFreeMode(RemoteMode::OFF);
#endif
}


int preferedDirection = Direction::TOP;

void loop()
{
    static Timer timer = Timer();

    // static unsigned i = 0;
    // bool showLogs = (i % 50 == 0);

#ifdef FREE_MODE
    static int currentTargetIndex = 0;
    if (navigation->driveTo(targets[currentTargetIndex], showLogs) == NAVIGATION_TARGET_STATE::REACHED)
    {
        gladiator->log("Target %d reached", currentTargetIndex);
        currentTargetIndex = (currentTargetIndex + 1) % (sizeof(targets) / sizeof(Vector2));
    }
#else
    if (gladiator->game->isStarted())
    {
        const MazeSquare *currentCase = gladiator->maze->getNearestSquare();
        if (timer.hasElapsed())
        {
            gladiator->log("shrinked");
            // gamestate->GetMazeData(gladiator.)
            gamestate->reduceMaze();
            gamestate->GetMazeData(gladiator);
            // msList->reset();
            timer.reset();
        }
        if (timer.mazeWillShrink() && timer.shrinkAlarmTriggered == false)
        {
            gladiator->log("will shrink");

            strategy->backOnMaze(msTransfo(currentCase), gamestate, msList);
            msList->reset();
            timer.shrinkAlarmTriggered = true;
            // msList->reset();
        }

        if (!msList->hasNext())
        {
            strategy->generatePath(20, nullptr, msTransfo(currentCase), msList, (Direction)preferedDirection);
            preferedDirection = (preferedDirection + 1) % 4;
            msList->printDebug();
        }

        if (caseCible == nullptr && msList->hasNext())
        {
            caseCible = msList->shift();
        }

        if (caseCible != nullptr)
        {
            if (navigation->driveTo(MyPosition(caseCible->i, caseCible->j).toVector(), false) == NAVIGATION_TARGET_STATE::REACHED)
            {
                caseCible = nullptr;
            }
        }


    }
#endif
    delay(10); // boucle à 100Hz
}