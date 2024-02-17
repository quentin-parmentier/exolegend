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

Gladiator *gladiator;
Navigation *navigation;

const MyPosition ROBOT_POSITION = MyPosition(0, 0); /// On peut peut être récupérer ça avec le Gladiator @todo

/// Position que l'on veut que notre robot parcours (On veut recalculer la nouvelle chaine quand il nous reste 2 positions dans la tab)
const int MAXIMAL_POSITION_ARRAY_LENGTH = DEPTH_WALKING + 2;
MyPosition *robotPositionArray[MAXIMAL_POSITION_ARRAY_LENGTH];

void reset()
{
}

// Pour tester le robot, ne pas oublier de commenter pour les matchs !!!
// #define FREE_MODE

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

bool isOnMazeBorder(int x, int y)
{
    /// Sur la borne côté
    if ((x == (MAZE_LENGTH - ACTUAL_MAZE_LENGTH) || x == (ACTUAL_MAZE_LENGTH - 1)) && y >= (MAZE_HEIGHT - ACTUAL_MAZE_HEIGHT) && y < ACTUAL_MAZE_HEIGHT)
    {
        return true;
    }
    /// Sur la borne haut/bas
    else if ((y == (MAZE_HEIGHT - ACTUAL_MAZE_HEIGHT) || y == (ACTUAL_MAZE_HEIGHT - 1)) && x >= (MAZE_LENGTH - ACTUAL_MAZE_LENGTH) && x < ACTUAL_MAZE_LENGTH)
    {
        return true;
    }

    return false;
}

bool isOutside(int x, int y)
{
    if (x < (MAZE_LENGTH - ACTUAL_MAZE_LENGTH) || x >= ACTUAL_MAZE_LENGTH || y < (MAZE_HEIGHT - ACTUAL_MAZE_HEIGHT) || y >= ACTUAL_MAZE_HEIGHT)
    {
        return true;
    }

    return false;
}

int valueOfMS(const MazeSquare *ms, const bool throughWall)
{
    int score = 0;

    const int caseAdverse = 50;
    const int caseNeutre = 20;
    const int caseEquipe = 0;
    const int caseRoquette = 20;
    const int caseDanger = -20;
    const int caseBorder = 70; /// Regarder qu'on ne l'a pas déjà ? @todo
    const int caseOustide = -500;
    const int caseGoingThrougWall = -40;

    if (ms == nullptr)
    {
        gladiator->log("MS EST NULL !! On est en dehors ?");
        return caseOustide;
    }

    /// Si la case a une roquette ++
    if ((ms->coin).value)
    {
        score += caseRoquette;
    }
    /// si case en danger --
    if (ms->danger)
    {
        score += caseDanger;
    }
    /// si case est vide ++
    if (ms->possession == 0)
    {
        score += caseNeutre;
    }
    /// Si la case est à nous --
    else if (ms->possession == currentRobotData.teamId)
    {
        score += caseEquipe;
    }
    /// Si la case est aux autre ++
    else
    {
        score += caseAdverse;
    }
    /// Si la case est sur le bord du maze
    if (isOnMazeBorder(ms->i, ms->j))
    {
        score += caseBorder;
    }
    if (isOutside(ms->i, ms->j))
    {
        score += caseOustide;
    }
    if (throughWall)
    {
        score += caseGoingThrougWall;
    }

    gladiator->log("SCORE : %d", score);

    return score;
    /// Si la case est près d'un ennemie et qu'on a pas de roquette --
    /// Si la case est près d'un ennemie (en ligne droite) et qu'on a une roquette ++
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

Direction getReverseDirection(Direction direction)
{
    if (direction == Direction::LEFT)
    {
        return Direction::RIGHT;
    }
    else if (direction == Direction::TOP)
    {
        return Direction::BOTTOM;
    }
    else if (direction == Direction::RIGHT)
    {
        return Direction::LEFT;
    }
    else if (direction == Direction::BOTTOM)
    {
        return Direction::TOP;
    }

    return Direction::TOP;
}

Direction getRandomDirection(Direction lastDirection)
{
    int8_t rand_index = random() % 4;

    gladiator->log("RANDOM: %d", rand_index);

    // Sélection aléatoire d'une valeur de l'énuméré
    Direction nextDirection = static_cast<Direction>(rand_index);

    // Si on retourne directe sur la case d'avant, on essaye d'avancer plutot
    if (getReverseDirection(nextDirection) == lastDirection)
    {
        nextDirection = lastDirection;
    }

    return nextDirection;
}

MyPosition getNextCase(Direction direction, MyPosition position)
{
    int actualX = position.getX();
    int actualY = position.getY();

    int nextX = actualX;
    int nextY = actualY;

    /// Si on va à droite x+1
    if (direction == Direction::RIGHT)
    {
        nextX = actualX + 1;
    } /// Si on va à gauche x-1
    else if (direction == Direction::LEFT)
    {
        nextX = actualX - 1;
    }
    /// Si on monte y+1
    else if (direction == Direction::TOP)
    {
        nextY = actualY + 1;
    }
    /// Si on descend y-1
    else if (direction == Direction::BOTTOM)
    {
        nextY = actualY - 1;
    }

    gladiator->log("FUTUR CASE %d:%d", nextX, nextY);

    return MyPosition(nextX, nextY);
}

bool goingThroughWall(Direction direction, const MazeSquare *mazeSquare)
{
    bool isGoingThroughWall = false;

    if (direction == Direction::TOP)
    {
        gladiator->log("TOP");
        if (mazeSquare != nullptr && mazeSquare->northSquare == nullptr)
        {
            gladiator->log("TOP A TRAVERS LE MUR");
            return true;
        }

        return false;
    }

    if (direction == Direction::LEFT)
    {
        gladiator->log("LEFT");
        if (mazeSquare != nullptr && mazeSquare->westSquare == nullptr)
        {
            gladiator->log("LEFT A TRAVERS LE MUR");
            return true;
        }

        return false;
    }

    if (direction == Direction::BOTTOM)
    {
        gladiator->log("BOTTOM");
        if (mazeSquare != nullptr && mazeSquare->southSquare == nullptr)
        {
            gladiator->log("BOTTOM A TRAVERS LE MUR");
            return true;
        }

        return false;
    }

    if (direction == Direction::RIGHT)
    {
        gladiator->log("RIGHT");
        if (mazeSquare != nullptr && mazeSquare->eastSquare == nullptr)
        {
            gladiator->log("RIGHT A TRAVERS LE MUR");
            return true;
        }

        return false;
    }

    gladiator->log("Traverse pas de mur");
    return isGoingThroughWall;
}

/// Pour l'instant on en fait rien, mais pourrait servir pour favoriser le fait d'aller tout droit
Direction actualDirection = Direction::LEFT;

void getRandomPathing(MyPosition *positionTabs, MyPosition fromPosition)
{
    const int numberOfTry = 10;
    int maxScore = 0;
    Direction lastDirection = actualDirection; /// On devrait peut être regarder fromPosition +

    /// La suite de position gagnante
    MyPosition positions[DEPTH_WALKING] = {
        MyPosition(0, 0),
        MyPosition(0, 0),
        MyPosition(0, 0),
        MyPosition(0, 0),
        MyPosition(0, 0),
    };

    for (int i = 0; i < numberOfTry; i++)
    {
        int scoreOfTry = 0;
        MyPosition positionOfTry[DEPTH_WALKING] = {
            MyPosition(0, 0),
            MyPosition(0, 0),
            MyPosition(0, 0),
            MyPosition(0, 0),
            MyPosition(0, 0),
        };
        MyPosition actualPosition = fromPosition;

        /// On tente d'avoir un parcours cool
        for (size_t j = 0; j < DEPTH_WALKING; j++)
        {
            /// On récupère une position random pour calculer sa valeur
            const Direction nextDirection = getRandomDirection(lastDirection);

            /// On ne peut pas forcément annuler le fait d'aller dans un mur parce que parfois ça sera la meilleure chose à faire
            /// Regarder si on est bloqué pour traverser un mur parce que la meilleure solution peut être de ne rien faire !
            const MyPosition nextPosition = getNextCase(nextDirection, actualPosition);
            const int x = nextPosition.getX();
            const int y = nextPosition.getY();
            gladiator->log("Next x : %d, Next y : %d", x, y);

            /// On ne veut pas aller en dehors de la Map => On passe
            if (isOutside(x, y))
            {
                gladiator->log("On est dehors");
                /// On fait en sorte de tjr avoir la bonne taille de MyPosition
                positionOfTry[j] = actualPosition;
                continue;
            }

            const int actualX = actualPosition.getX();
            const int actualY = actualPosition.getY();
            gladiator->log("Actual x : %d, Actual y : %d", actualX, actualY);

            /// On récupère la valeur du prochain MazeSquare
            const MazeSquare *nextMazeSquare = maze[x][y];
            const MazeSquare *actualMazeSquare = maze[actualX][actualY];

            const bool isGoingThroughWall = goingThroughWall(nextDirection, actualMazeSquare);
            gladiator->log("Though wall : %d", isGoingThroughWall);

            const int valueOfNextMazeSquare = valueOfMS(nextMazeSquare, isGoingThroughWall);
            gladiator->log("VALEUR: %d", valueOfNextMazeSquare);

            gladiator->log("NEXT %d", nextDirection);
            gladiator->log("NEXT %d:%d", nextPosition.getX(), nextPosition.getY());
            /// On update
            actualPosition = nextPosition;
            lastDirection = nextDirection;
            scoreOfTry += valueOfNextMazeSquare;
            positionOfTry[j] = nextPosition;
        }

        if (maxScore < scoreOfTry)
        {
            maxScore = scoreOfTry;
            for (size_t i = 0; i < DEPTH_WALKING; i++)
            {
                positions[i] = positionOfTry[i];
            }
        }
    }

    gladiator->log("On met à jour notre tableau passé en params");
    for (int i = 0; i < DEPTH_WALKING; i++)
    {
        gladiator->log("La pos %d c'est %d:%d", i, positions[i].getX(), positions[i].getY());
        positionTabs[i] = positions[i];
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
        ///@todo: Voir avec JB
        MyPosition positionTab[DEPTH_WALKING] = {
            MyPosition(0, 0),
            MyPosition(0, 0),
            MyPosition(0, 0),
            MyPosition(0, 0),
            MyPosition(0, 0),
        };

        getRandomPathing(positionTab, ROBOT_POSITION); // @todo Premiere itération ROBOT_POSITION puis derniere case dans robotPositionArray

        for (size_t i = 0; i < DEPTH_WALKING; i++)
        {
            /// On ajoute ces positions aux positions que doit parcourir notre robot [robotPositionArray]

            gladiator->log("NEXT Case : %d:%d", positionTab[i].getX(), positionTab[i].getY());
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
    }

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
    if (gladiator->game->isStarted())
    {
        // TODO : gagner
    }
#endif
    delay(10); // boucle à 100Hz
}