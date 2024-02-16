#include "gladiator.h"
#include "Vector2.hpp"
#include "Navigation.hpp"
#include "utils.hpp"

// #include <random>
#include <algorithm> // Pour std::copy
#include <iostream>
#include "MyPosition.hpp"
#include "Timer.hpp"

RobotData currentRobotData;

#undef abs

const int MAZE_HEIGHT = 12;
const int MAZE_LENGTH = 12;
int ACTUAL_MAZE_HEIGHT = MAZE_HEIGHT;
int ACTUAL_MAZE_LENGTH = MAZE_LENGTH;
const MazeSquare *maze[MAZE_HEIGHT][MAZE_LENGTH];
const int DEPTH_WALKING = 5;

const MyPosition ROBOT_POSITION = MyPosition(0, 0); /// On peut peut être récupérer ça avec le Gladiator @todo
Gladiator *gladiator;
Navigation *navigation;

void reset()
{
}

// Pour tester le robot, ne pas oublier de commenter pour les matchs !!!
//#define FREE_MODE

void setup()
{
    // instanciation de l'objet gladiator
    gladiator = new Gladiator();
    // enregistrement de la fonction de reset qui s'éxecute à chaque fois avant qu'une partie commence
    gladiator->game->onReset(&reset);
    // gladiator->game->enableFreeMode(RemoteMode::OFF);
    navigation = new Navigation(gladiator);

    #ifdef FREE_MODE
        gladiator->game->enableFreeMode(RemoteMode::OFF);
    #endif

    // recuperer les informations du robot
    currentRobotData = gladiator->robot->getData();
}

bool isOnMazeBorder(int i, int j)
{
    if (i == (MAZE_LENGTH - ACTUAL_MAZE_LENGTH) / 2 || i == ACTUAL_MAZE_LENGTH + (MAZE_LENGTH - ACTUAL_MAZE_LENGTH) / 2 - 1)
    {
        return true;
    }
    else if (j == (MAZE_HEIGHT - ACTUAL_MAZE_HEIGHT) / 2 || j == ACTUAL_MAZE_HEIGHT + (MAZE_HEIGHT - ACTUAL_MAZE_HEIGHT) / 2 - 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool isOutside(int i, int j)
{
    if (i < (MAZE_LENGTH - ACTUAL_MAZE_LENGTH) / 2 || i >= ACTUAL_MAZE_LENGTH + (MAZE_LENGTH - ACTUAL_MAZE_LENGTH) / 2 - 1)
    {
        return true;
    }
    else if (j < (MAZE_HEIGHT - ACTUAL_MAZE_HEIGHT) / 2 || j >= ACTUAL_MAZE_HEIGHT + (MAZE_HEIGHT - ACTUAL_MAZE_HEIGHT) / 2 - 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int valueOfMS(const MazeSquare *ms, const bool throughWall)
{
    // return 1;
    int score = 0;

    const int caseAdverse = 15;
    const int caseEquipe = 0;
    const int caseRoquette = 20;
    const int caseDanger = -20;
    const int caseNeutre = 10;
    const int caseBorder = 20;
    const int caseOustide = -500;
    const int caseGoingThrougWall = -40;

    /// Si la case a une roquette ++
    if ((ms->coin).value)
    {
        score += caseRoquette;
    }
    /// si case en danger --
    if (ms->danger)
        score += caseDanger;
    /// si case est vide ++
    if (ms->possession == 0)
        score += caseNeutre;
    /// Si la case est à nous --
    else if (ms->possession == currentRobotData.teamId)
        score += caseEquipe;
    /// Si la case est aux autre ++
    else
        score += caseAdverse;
    /// Si la case est sur le bord du maze
    if (isOnMazeBorder(ms->i, ms->j))
        score += caseBorder;

    if (isOutside(ms->i, ms->j))
        score += caseOustide;

    if (throughWall)
        score += caseGoingThrougWall;

            return score;
    /// Si la case est près d'un ennemie et qu'on a pas de roquette --
    /// Si la case est près d'un ennemie (en ligne droite) et qu'on a une roquette ++

    return 1;
}

int costOfMS(MazeSquare ms)
{
    /// Pour l'instant on ignore les couts
    return 1;

    /// On peut se dire qu'aller dans le même sens que ce qu'on a déjà coute moins cher que de tourner et encore moins cher qu'un demi tour
}

enum Direction
{
    LEFT,
    RIGHT,
    TOP,
    BOTTOM,
};

Direction getRandomDirection()
{
    int8_t rand_index = random() % 4;

    gladiator->log("RANDOM: %d", rand_index);

    // Sélection aléatoire d'une valeur de l'énuméré
    return static_cast<Direction>(rand_index);
}

MyPosition getNextCase(Direction direction, MyPosition position)
{
    /// On récupère la position d'après
    return position;
}

bool goingThroughWall(Direction direction, const MazeSquare *mazeSquare)
{
    bool isGoingThroughWall = false;

    if (mazeSquare == nullptr)
    {
        gladiator->log("On fait de la merde");
    }

    if (direction == Direction::TOP)
    {
        gladiator->log("TOP");
        if (mazeSquare != nullptr && mazeSquare->northSquare == nullptr)
        {
            gladiator->log("TOP");
            return true;
        }

        return false;
    }

    if (direction == Direction::LEFT)
    {
        gladiator->log("LEFT");
        if (mazeSquare != nullptr && mazeSquare->westSquare == nullptr)
        {
            gladiator->log("LEFT");
            return true;
        }

        return false;
    }

    if (direction == Direction::BOTTOM)
    {
        gladiator->log("BOTTOM");
        if (mazeSquare != nullptr && mazeSquare->southSquare == nullptr)
        {
            gladiator->log("BOTTOM");
            return true;
        }

        return false;
    }

    if (direction == Direction::RIGHT)
    {
        gladiator->log("RIGHT");
        if (mazeSquare != nullptr && mazeSquare->eastSquare == nullptr)
        {
            gladiator->log("RIGHT");
            return true;
        }

        return false;
    }

    gladiator->log("Traverse pas de mur");
    return isGoingThroughWall;
}

Direction actualDirection = Direction::LEFT;

void getRandomPathing(Direction *directionTab)
{
    const int numberOfTry = 10;
    int maxScore = 0;
    Direction directions[DEPTH_WALKING] = {};

    for (int i = 0; i < numberOfTry; i++)
    {
        int scoreOfTry = 0;
        Direction directionsOfTry[DEPTH_WALKING] = {};
        MyPosition actualPosition = ROBOT_POSITION;
        /// On tente d'avoir un parcours cool
        for (size_t j = 0; j < DEPTH_WALKING; j++)
        {
            /// On récupère une position random pour calculer sa valeur
            const Direction nextDirection = getRandomDirection();

            /// On ne peut pas forcément annuler le fait d'aller dans un mur parce que parfois ça sera la meilleure chose à faire
            /// Regarder si on est bloqué pour traverser un mur parce que la meilleure solution peut être de ne rien faire !
            const MyPosition nextPosition = getNextCase(nextDirection, actualPosition);
            const int x = nextPosition.getX();
            const int y = nextPosition.getY();

            gladiator->log("Next x : %d, Next y : %d", x, y);

            const int actualX = actualPosition.getX();
            const int actualY = actualPosition.getY();

            gladiator->log("Actual x : %d, Actual y : %d", actualX, actualY);

            const MazeSquare *nextMazeSquare = maze[x][y];
            const MazeSquare *actualMazeSquare = maze[actualX][actualY];

            const bool isGoingThroughWall = goingThroughWall(nextDirection, actualMazeSquare);

            gladiator->log("Though wall : %d", isGoingThroughWall);

            /// On récupère le prochain MazeSquare
            const int valueOfNextMazeSquare = valueOfMS(nextMazeSquare, isGoingThroughWall);

            gladiator->log("VALEUR: %d", valueOfNextMazeSquare);

            /// On update
            actualPosition = nextPosition;
            scoreOfTry += valueOfNextMazeSquare;
            directionsOfTry[j] = nextDirection;

            gladiator->log("NEXT %d", nextDirection);
        }

        if (maxScore < scoreOfTry)
        {
            maxScore = scoreOfTry;
            for (size_t i = 0; i < DEPTH_WALKING; i++)
            {
                directions[i] = directionsOfTry[i];
            }
            // std::copy(std::begin(directionsOfTry), std::end(directionsOfTry), std::begin(directions));
        }
    }

    for (size_t i = 0; i < DEPTH_WALKING; i++)
    {
        directionTab[i] = directions[i];
    }
}

bool isFirst = true;

MyPosition pos = MyPosition(1, 0);
const Vector2 targets[] = {
    Vector2(0., .5),
    Vector2(.5, .5),
    Vector2(.5, 0.),
    Vector2(0., 0.),
};

void loop()
{
    if (isFirst)
    {
        if (gladiator->maze == nullptr)
        {
            gladiator->log("MAZE EST NULL");
        }

        // On récupère tous les points
        for (int x = 0; x < MAZE_LENGTH; x++)
        {
            for (int y = 0; y < MAZE_HEIGHT; y++)
            {
                const MazeSquare *msq = gladiator->maze->getSquare(x, y);
                maze[x][y] = msq;
            }
        }

        gladiator->log("STRAT TEST");
        isFirst = false;
        Direction direction[DEPTH_WALKING] = {};

        getRandomPathing(direction);

        for (size_t i = 0; i < DEPTH_WALKING; i++)
        {
            std::cout << "Valeur de Direction : " << static_cast<int>(direction[i]) << std::endl;
            gladiator->log("PATH : %d", direction[i]);
        }
    }

    if (gladiator->game->isStarted())
    {
        /// On set la vrai direction du robot
        /// actualDirection = Direction::?? @todo
        /// ROBOT_POSITION aussi : @todo

        static Timer timer = Timer();

        if (timer.hasElapsed())
        {
            ACTUAL_MAZE_HEIGHT = ACTUAL_MAZE_HEIGHT - 2;
            ACTUAL_MAZE_LENGTH = ACTUAL_MAZE_LENGTH - 2;
            timer.reset();
            // gladiator->log("ça fait 20 secondes");
        }
        else
        {
            // gladiator->log("Pas encore 20 secondes");
        }

        MyPosition pos = MyPosition(5, 5);

        static unsigned i = 0;
        bool showLogs = (i % 50 == 0);

        i++;
    }

    static unsigned i = 0;
    bool showLogs = (i % 50 == 0);
    static int currentTargetIndex = 0;

    #ifdef FREE_MODE
        if (navigation->driveTo(targets[currentTargetIndex], showLogs) == NAVIGATION_TARGET_STATE::REACHED) {
            gladiator->log("Target %d reached", currentTargetIndex);
            currentTargetIndex = (currentTargetIndex + 1) % (sizeof(targets) / sizeof(Vector2));
        }
    #else 
        if (gladiator->game->isStarted())
        {
            // TODO : gagner
        }
    #endif
    delay(10); // boucle à 100Hz
}