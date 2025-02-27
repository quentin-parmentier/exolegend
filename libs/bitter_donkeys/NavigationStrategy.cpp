#include "NavigationStrategy.hpp"
#include "utils.hpp"

NavigationStrategy::NavigationStrategy(NavigationStack *navigationStack, Gladiator *gladiator, int depthWalking, const MazeSquare ***maze, int *mazeHeight, int *mazeLength, int originalMazeHeight, int originalMazeLength)
    : navigationStack(navigationStack), gladiator(gladiator), depthWalking(depthWalking), maze(maze), mazeHeight(mazeHeight), mazeLength(mazeLength), originalMazeHeight(originalMazeHeight), originalMazeLength(originalMazeLength)
{
}

void NavigationStrategy::computeBestPath(
    MyPosition actualPos, 
    std::vector<MyPosition> currentPath, 
    std::vector<MyPosition>& bestPath, 
    int currentScore, 
    int& maxScore
) {
    if (navigationStack->hasNext()) {
        return;
    }

    currentPath.push_back(actualPos);

    int depthRemaining = depthWalking - currentPath.size();

    if (depthRemaining == 0) {
        if (currentScore > maxScore) {
            maxScore = currentScore;
            bestPath = currentPath; // Mise à jour du meilleur chemin
        }
        return;
    }

    for (Direction dir : {LEFT, RIGHT, TOP, BOTTOM}) {
        MyPosition nextPos = getNextCase(dir, actualPos);
        if (isOutside(nextPos.getX(), nextPos.getY())) continue;

        const bool isGoingThroughWall = goingThroughWall(dir, maze[actualPos.getX()][actualPos.getY()]);
        int nextValue = valueOfMS(maze[nextPos.getX()][nextPos.getY()], isGoingThroughWall, currentPath);

        computeBestPath(nextPos, currentPath, bestPath, currentScore + nextValue, maxScore);
    }

    currentPath.pop_back();

    if (currentPath.empty()) { // Ça signifie qu'on est revenu à la racine (premier appel)
        navigationStack->reset();
        for (const auto& pos : bestPath) {
            navigationStack->push(pos);
        }
    }
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

int NavigationStrategy::valueOfMS(const MazeSquare *ms, const bool throughWall, std::vector<MyPosition>& visited)
{
    int score = 0;

    const int caseEquipe = -500;
    const int caseNeutre = 200;
    const int caseAdverse = 400;

    const int caseRoquette = 100;
    const int caseBorder = 200;

    const int caseDanger = -20;
    const int caseGoingThrougWall = -1100;

    const int caseOustide = -1000000;

    if (ms == nullptr)
    {
        return caseOustide;
    }

    bool found = std::find_if(visited.begin(), visited.end(), [ms](const MyPosition& p) {
        return p.getX() == ms->i && p.getY() == ms->j;
    }) != visited.end();

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
    if (ms->possession == gladiator->robot->getData().teamId || found) {
        score += caseEquipe;
    }
    else if (ms->possession == 0)
    {
        score += caseNeutre;
    }  
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