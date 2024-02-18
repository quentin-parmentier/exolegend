#include "gladiator.h"
#include "Vector2.hpp"
#include "Navigation.hpp"
#include "utils.hpp"

// #include <random>
#include <algorithm> // Pour std::copy
#include <iostream>
#include "MyPosition.hpp"
#include "Timer.hpp"
#include "NavigationStack.hpp"
#include "NavigationStrategy.hpp"
#include "StateStrategy.hpp"
#include "RobotsData.hpp"

#undef abs

const int MAZE_HEIGHT = 12; // y
const int MAZE_LENGTH = 12; // x
int ACTUAL_MAZE_HEIGHT = MAZE_HEIGHT;
int ACTUAL_MAZE_LENGTH = MAZE_LENGTH;
const MazeSquare ***maze;
const int DEPTH_WALKING = 10;

bool isFirst = true;
bool firstTimerElapsed = false;

Gladiator *gladiator;
Navigation *navigation;
StateStrategy *stateStrategy;
RobotsData *robotsData;

MyPosition ROBOT_POSITION = MyPosition(0, 0);

/// Position que l'on veut que notre robot parcours (On veut recalculer la nouvelle chaine quand il nous reste 2 positions dans la tab)
const int MAXIMAL_POSITION_ARRAY_LENGTH = DEPTH_WALKING + 2;
MyPosition *robotPositionArray[MAXIMAL_POSITION_ARRAY_LENGTH];
NavigationStack *navigationStack;
NavigationStrategy *navigationStrategy;

// Pour tester le robot, ne pas oublier de commenter pour les matchs !!!
// #define FREE_MODE

/// Pour l'instant on en fait rien, mais pourrait servir pour favoriser le fait d'aller tout droit
Direction actualDirection = Direction::LEFT;

void reset()
{
    navigationStack->reset();
    ACTUAL_MAZE_HEIGHT = MAZE_HEIGHT;
    ACTUAL_MAZE_LENGTH = MAZE_LENGTH;
    // On récupère tous les points
    for (int x = 0; x < MAZE_LENGTH; x++)
    {
        for (int y = 0; y < MAZE_HEIGHT; y++)
        {
            const MazeSquare *msq = gladiator->maze->getSquare(x, y);
            maze[x][y] = msq;
        }
    }
    isFirst = true;

    const MazeSquare *initialMazeSquare = gladiator->maze->getNearestSquare();
    ROBOT_POSITION = MyPosition(initialMazeSquare->i, initialMazeSquare->j);

    gladiator->log("ROBOT BASE %d:%d", ROBOT_POSITION.getX(), ROBOT_POSITION.getY());

    navigationStrategy->computeRandomPathing(ROBOT_POSITION); // @todo Premiere itération ROBOT_POSITION puis derniere case dans robotPositionArray
    stateStrategy->actualPositionToFind = navigationStack->shift();
    firstTimerElapsed = false;
}

void setup()
{
    maze = (const MazeSquare ***)malloc(sizeof(MazeSquare **) * MAZE_LENGTH);
    for (int i = 0; i < MAZE_LENGTH; i++)
    {
        maze[i] = (const MazeSquare **)malloc(sizeof(MazeSquare *) * MAZE_HEIGHT);
    }

    // instanciation de l'objet gladiator
    gladiator = new Gladiator();
    // enregistrement de la fonction de reset qui s'éxecute à chaque fois avant qu'une partie commence
    gladiator->game->onReset(&reset);
    // gladiator->game->enableFreeMode(RemoteMode::OFF);
    navigation = new Navigation(gladiator);
    robotsData = new RobotsData(gladiator);
    navigationStack = new NavigationStack(MAXIMAL_POSITION_ARRAY_LENGTH);
    navigationStrategy = new NavigationStrategy(navigationStack, gladiator, DEPTH_WALKING, maze, &ACTUAL_MAZE_HEIGHT, &ACTUAL_MAZE_LENGTH, MAZE_HEIGHT, MAZE_LENGTH);
    stateStrategy = new StateStrategy(STATE::BASIC, robotsData, navigation, navigationStack, gladiator, navigationStrategy,
                                      MAZE_HEIGHT,
                                      MAZE_LENGTH,
                                      &ACTUAL_MAZE_HEIGHT,
                                      &ACTUAL_MAZE_LENGTH);

#ifdef FREE_MODE
    gladiator->game->enableFreeMode(RemoteMode::OFF);
#endif
}

void loop()
{

#ifdef FREE_MODE
    static unsigned i = 0;
    static int currentTargetIndex = 0;
    bool showLogs = (i % 50 == 0);
    if (navigation->driveTo(targets[currentTargetIndex], showLogs) == NAVIGATION_TARGET_STATE::REACHED)
    {
        gladiator->log("Target %d reached", currentTargetIndex);
        currentTargetIndex = (currentTargetIndex + 1) % (sizeof(targets) / sizeof(Vector2));
    }
    i++;
#else
    static Timer timer18sec = Timer(18);
    static Timer timer20sec = Timer(20);
    if (gladiator->game->isStarted())
    {
        if(isFirst){
            robotsData->init();
            isFirst = false;
        }
    
        if (
            (!firstTimerElapsed && timer18sec.hasElapsed()) ||
            (firstTimerElapsed && timer20sec.hasElapsed())
        )
        {
            ACTUAL_MAZE_HEIGHT = ACTUAL_MAZE_HEIGHT - 2;
            ACTUAL_MAZE_LENGTH = ACTUAL_MAZE_LENGTH - 2;
            firstTimerElapsed = true;
            timer20sec.reset();

            if (stateStrategy->state != STATE::SAVE)
            {
                /// On recréer notre liste comme il faut
                gladiator->log("C'est elapse");
                stateStrategy->resetBasicStrategy();
            }
            else
            {
                gladiator->log("On essaye toujours de se barrer");
            }
        }

        stateStrategy->next(false);
    }
    else
    {
        timer18sec.reset();
        timer20sec.reset();
    }
    delay(10); // boucle à 100Hz
#endif
}