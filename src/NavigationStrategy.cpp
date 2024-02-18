#include "NavigationStrategy.hpp"
#include "utils.hpp"

NavigationStrategy::NavigationStrategy(NavigationStack *navigationStack, Gladiator *gladiator, int depthWalking, const MazeSquare ***maze, int *mazeHeight, int *mazeLength, int originalMazeHeight, int originalMazeLength)
    : navigationStack(navigationStack), gladiator(gladiator), depthWalking(depthWalking), maze(maze), mazeHeight(mazeHeight), mazeLength(mazeLength), originalMazeHeight(originalMazeHeight), originalMazeLength(originalMazeLength)
{
}

void NavigationStrategy::computeRandomPathing(MyPosition fromPosition)
{
    // Si on a encore assez de chemin faire on calcule pas de nouveau chemin
    if (navigationStack->hasNext())
    {
        gladiator->log("On a pas encore tout depile");
        return;
    }

    gladiator->log("On relance une recherche");

    const int numberOfTry = 65;
    int maxScore = 0;
    /// On va ignorer la premiere direction parce que c'est compliqué de savoir dans x pas à combien on sera @todo
    Direction lastDirection = Direction::RIGHT;

    /// On devrait peut être regarder fromPosition +

    /// La suite de position gagnante
    MyPosition positions[depthWalking];

    for (int i = 0; i < numberOfTry; i++)
    {
        int scoreOfTry = 0;
        MyPosition positionOfTry[depthWalking];

        MyPosition actualPosition = fromPosition;

        gladiator->log("actualPosition est %d:%d", actualPosition.getX(), actualPosition.getY());

        /// On tente d'avoir un parcours cool
        for (int j = 0; j < depthWalking; j++)
        {
            /// On récupère une position random pour calculer sa valeur
            const Direction nextDirection = getRandomDirection(lastDirection, j != 0);

            /// On ne peut pas forcément annuler le fait d'aller dans un mur parce que parfois ça sera la meilleure chose à faire
            /// Regarder si on est bloqué pour traverser un mur parce que la meilleure solution peut être de ne rien faire !
            const MyPosition nextPosition = getNextCase(nextDirection, actualPosition);
            const int x = nextPosition.getX();
            const int y = nextPosition.getY();

            /// On ne veut pas aller en dehors de la Map => On passe
            if (isOutside(x, y))
            {
                // gladiator->log("%d:%d est OUT", x, y);
                /// On fait en sorte de tjr avoir la bonne taille de MyPosition
                j--;
                continue;
            }
            else
            {
                // gladiator->log("%d:%d est DEDANS", x, y);
            }

            const int actualX = actualPosition.getX();
            const int actualY = actualPosition.getY();

            /// On récupère la valeur du prochain MazeSquare
            const MazeSquare *nextMazeSquare = maze[x][y];
            const MazeSquare *actualMazeSquare = maze[actualX][actualY];

            const bool isGoingThroughWall = goingThroughWall(nextDirection, actualMazeSquare);
            const int valueOfNextMazeSquare = valueOfMS(nextMazeSquare, isGoingThroughWall);

            /// On update
            actualPosition = nextPosition;
            lastDirection = nextDirection;
            scoreOfTry += valueOfNextMazeSquare;
            positionOfTry[j] = nextPosition;
        }

        if (maxScore < scoreOfTry)
        {
            maxScore = scoreOfTry;
            for (int i = 0; i < depthWalking; i++)
            {
                positions[i] = positionOfTry[i];
            }
        }
    }

    for (int i = 0; i < depthWalking; i++)
    {
        navigationStack->push(positions[i]);
    }

    // navigationStack->printTab();

    // navigationStack->simplify();
}

// Pour tester le robot, ne pas oublier de commenter pour les matchs !!!
// #define FREE_MODE

bool NavigationStrategy::isOnMazeBorder(int x, int y)
{
    /// Sur la borne côté
    if ((x == (originalMazeLength - (*mazeLength)) / 2 || x == ((*mazeLength)) - 1) && y >= (originalMazeHeight - (*mazeHeight)) / 2 && y < (*mazeHeight))
    {
        return true;
    }
    /// Sur la borne haut/bas
    else if ((y == (originalMazeHeight - (*mazeHeight)) / 2 || y == ((*mazeHeight) - 1)) && x >= (originalMazeLength - (*mazeLength)) / 2 && x < (*mazeLength))
    {
        return true;
    }

    return false;
}

bool NavigationStrategy::isOutside(int x, int y)
{
    return checkIfIsOutside(x, y, originalMazeHeight,
                            originalMazeLength,
                            mazeHeight,
                            mazeLength);
}

int NavigationStrategy::valueOfMS(const MazeSquare *ms, const bool throughWall)
{
    int score = 0;

    const int caseEquipe = -20;
    const int caseNeutre = 10;
    const int caseAdverse = 20;

    const int caseRoquette = 50;
    const int caseBorder = 40;

    const int caseDanger = -20;
    const int caseGoingThrougWall = -300;

    const int caseOustide = -10000;

    if (ms == nullptr)
    {
        return caseOustide;
    }

    /// Si la case est à nous --
    if (ms->possession == gladiator->robot->getData().teamId)
    {
        return caseEquipe;
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

    return score;
    /// Si la case est près d'un ennemie et qu'on a pas de roquette --
    /// Si la case est près d'un ennemie (en ligne droite) et qu'on a une roquette ++
}

int NavigationStrategy::costOfMS(MazeSquare ms)
{
    /// Pour l'instant on ignore les couts
    return 1;

    /// On peut se dire qu'aller dans le même sens que ce qu'on a déjà coute moins cher que de tourner et encore moins cher qu'un demi tour
}

Direction NavigationStrategy::getReverseDirection(Direction direction)
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

Direction NavigationStrategy::getRandomDirection(Direction lastDirection, bool tryToGoForward)
{
    int8_t rand_index = random() % 4;

    // Sélection aléatoire d'une valeur de l'énuméré
    Direction nextDirection = static_cast<Direction>(rand_index);

    // Si on retourne directe sur la case d'avant, on essaye d'avancer plutot avec une probabilité de 30%
    if (tryToGoForward && getReverseDirection(nextDirection) == lastDirection && (random() % 10) <= 3)
    {
        nextDirection = lastDirection; //@todo a remettre
    }

    return nextDirection;
}

MyPosition NavigationStrategy::getNextCase(Direction direction, MyPosition position)
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

    return MyPosition(nextX, nextY);
}

bool NavigationStrategy::goingThroughWall(Direction direction, const MazeSquare *mazeSquare)
{
    bool isGoingThroughWall = false;

    if (direction == Direction::TOP)
    {
        if (mazeSquare != nullptr && mazeSquare->northSquare == nullptr)
        {
            return true;
        }

        return false;
    }

    if (direction == Direction::LEFT)
    {
        if (mazeSquare != nullptr && mazeSquare->westSquare == nullptr)
        {
            return true;
        }

        return false;
    }

    if (direction == Direction::BOTTOM)
    {
        if (mazeSquare != nullptr && mazeSquare->southSquare == nullptr)
        {
            return true;
        }

        return false;
    }

    if (direction == Direction::RIGHT)
    {
        if (mazeSquare != nullptr && mazeSquare->eastSquare == nullptr)
        {
            return true;
        }

        return false;
    }

    return isGoingThroughWall;
}