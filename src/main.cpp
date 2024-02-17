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

#undef abs

const int MAZE_HEIGHT = 12; // y
const int MAZE_LENGTH = 12; // x
int ACTUAL_MAZE_HEIGHT = MAZE_HEIGHT;
int ACTUAL_MAZE_LENGTH = MAZE_LENGTH;
const MazeSquare ***maze;
const int DEPTH_WALKING = 15;

Gladiator *gladiator;
Navigation *navigation;

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

MyPosition pos = MyPosition(1, 0);
const Vector2 targets[] = {
    Vector2(0., .5),
    Vector2(.5, .5),
    Vector2(.5, 0.),
    Vector2(0., 0.),
};

MyPosition actualPositionToFind = MyPosition();

void reset()
{
    navigationStack->reset();
    // On récupère tous les points
    for (int x = 0; x < MAZE_LENGTH; x++)
    {
        for (int y = 0; y < MAZE_HEIGHT; y++)
        {
            const MazeSquare *msq = gladiator->maze->getSquare(x, y);
            maze[x][y] = msq;
        }
    }

    const MazeSquare *initialMazeSquare = gladiator->maze->getNearestSquare();
    ROBOT_POSITION = MyPosition(initialMazeSquare->i, initialMazeSquare->j);

    gladiator->log("ROBOT BASE %d:%d", ROBOT_POSITION.getX(), ROBOT_POSITION.getY());

    navigationStrategy->computeRandomPathing(ROBOT_POSITION); // @todo Premiere itération ROBOT_POSITION puis derniere case dans robotPositionArray
    actualPositionToFind = navigationStack->shift();
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
    navigationStack = new NavigationStack(MAXIMAL_POSITION_ARRAY_LENGTH);
    navigationStrategy = new NavigationStrategy(navigationStack, gladiator, DEPTH_WALKING, maze, &ACTUAL_MAZE_HEIGHT, &ACTUAL_MAZE_LENGTH, MAZE_HEIGHT, MAZE_LENGTH);

#ifdef FREE_MODE
    gladiator->game->enableFreeMode(RemoteMode::OFF);
#endif
}

void loop()
{
    static unsigned i = 0;
    bool showLogs = (i % 100 == 0);
    i++;

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
    static Timer timer = Timer();

    if (timer.hasElapsed())
    {
        ACTUAL_MAZE_HEIGHT = ACTUAL_MAZE_HEIGHT - 2;
        ACTUAL_MAZE_LENGTH = ACTUAL_MAZE_LENGTH - 2;
        timer.reset();

        /// Notre stack n'est plus bonne on la reset
        navigationStack->reset();

        /// On se sauve le cul si jamais on est pas dedans 1s avant

        /// On recréer notre liste comme il faut
        const MazeSquare *initialMazeSquare = gladiator->maze->getNearestSquare();
        const MyPosition actualRobotPosition = MyPosition(initialMazeSquare->i, initialMazeSquare->j);
        gladiator->log("Actual position %d:%d", actualRobotPosition.getX(), actualRobotPosition.getY());
        navigationStrategy->computeRandomPathing(actualRobotPosition);
    }
    else
    {
        /// On va vers la position qu'on vire
        NAVIGATION_TARGET_STATE navigationState = navigation->driveTo(actualPositionToFind.toVector(gladiator->maze->getSquareSize()));

        if (showLogs)
        {
            gladiator->log("Position visée %d:%d", actualPositionToFind.getX(), actualPositionToFind.getY());
        }

        /// On attend de voir si on est arrivé pour redonner la prochaine case
        if (navigationState == NAVIGATION_TARGET_STATE::REACHED)
        {
            gladiator->log("Target %d:%d reached", actualPositionToFind.getX(), actualPositionToFind.getY());

            /// On regarde la prochaine position à aller voir
            actualPositionToFind = navigationStack->shift();
            MyPosition *positionOnTop = navigationStack->getPositionOnTop();
            navigationStrategy->computeRandomPathing(*positionOnTop);
        }
    }
#endif
    delay(10); // boucle à 100Hz
}