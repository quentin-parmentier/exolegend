#include "StateStrategy.hpp"
#include "MyPosition.hpp"
#include "RobotsData.hpp"
#include "utils.hpp"

StateStrategy::StateStrategy(STATE baseState, RobotsData *robotsData, Navigation *navigation, NavigationStack *navigationStack, Gladiator *gladiator, NavigationStrategy *navigationStrategy, int originalMazeHeight, int originalMazeLength, int *mazeHeight, int *mazeLength) : state(baseState), robotsData(robotsData), navigation(navigation),
                                                                                                                                                                                                                                                                                  navigationStack(navigationStack),
                                                                                                                                                                                                                                                                                  gladiator(gladiator), navigationStrategy(navigationStrategy), originalMazeHeight(originalMazeHeight),
                                                                                                                                                                                                                                                                                  originalMazeLength(originalMazeLength), mazeHeight(mazeHeight),
                                                                                                                                                                                                                                                                                  mazeLength(mazeLength)
{
    actualPositionToFind = MyPosition();
    savePosition = MyPosition();
    hasToGoToSavePosition = false;
};

void StateStrategy::resetBasicStrategy()
{
    gladiator->log("On reset");
    /// Notre stack n'est plus bonne on la reset
    navigationStack->reset();
    const MazeSquare *initialMazeSquare = gladiator->maze->getNearestSquare();
    const MyPosition actualRobotPosition = MyPosition(initialMazeSquare->i, initialMazeSquare->j);
    gladiator->log("Actual position %d:%d", actualRobotPosition.getX(), actualRobotPosition.getY());
    navigationStrategy->computeRandomPathing(actualRobotPosition);
}

void StateStrategy::next(bool mazeWillShrink)
{
    /// On fait les différents checks
    if (mazeWillShrink && shouldSaveMyAss())
    {
        state = STATE::SAVE;
    }
    else if (shouldLaunchRocket(actualPositionToFind))
    {
        state = STATE::ROCKET;
    }
    else if (robotsData->isEnemyClose(0.4))
    {
        // gladiator->log("defend");
        state = STATE::DEFEND;
    }
    else if (state != STATE::BASIC)
    {
        resetBasicStrategy();
        state = STATE::BASIC;
    }

    /// On fait l'action en fonction de l'état
    if (state == STATE::BASIC)
    {
        useBasicStrategy();
    }
    else if (state == STATE::DEFEND)
    {
        useSpinStrategy();
    }
    else if (state == STATE::SAVE)
    {
        useSaveStrategy();
    }
    else if (state == STATE::ROCKET)
    {
        useRocketStrategy();
    }
}

void StateStrategy::useBasicStrategy()
{
    /// On va vers la position qu'on vire
    NAVIGATION_TARGET_STATE navigationState = navigation->driveTo(actualPositionToFind.toVector(gladiator->maze->getSquareSize()));

    /// On attend de voir si on est arrivé pour redonner la prochaine case
    if (navigationState == NAVIGATION_TARGET_STATE::REACHED)
    {
        gladiator->log("Target %d:%d reached", actualPositionToFind.getX(), actualPositionToFind.getY());

        /// On regarde la prochaine position à aller voir
        if (navigationStack->hasNext()) {
            actualPositionToFind = navigationStack->shift();
        }
        navigationStrategy->computeRandomPathing(actualPositionToFind);
    }
};

void StateStrategy::useSpinStrategy()
{
    navigation->spin();
}

void StateStrategy::useSaveStrategy()
{
    /// On vient d'arriver dans l'état SAVE
    if (!hasToGoToSavePosition)
    {
        hasToGoToSavePosition = true;
        /// On trouve la case la plus proche
        const MazeSquare *msq = gladiator->maze->getNearestSquare();

        int nextMazeHeight = (*mazeHeight - 2);
        int nextMazeLength = (*mazeLength - 2);
        savePosition = getNearestValidPosition(msq->i, msq->j, originalMazeHeight, originalMazeLength, nextMazeHeight, nextMazeLength);

        gladiator->log("On est en : %d:%d", msq->i, msq->j);
        gladiator->log("La position de repli c'est : %d:%d", savePosition.getX(), savePosition.getY());
    }

    /// On y va à pleine balle tout droit
    NAVIGATION_TARGET_STATE navigationState = navigation->driveTo(savePosition.toVector(gladiator->maze->getSquareSize()));
    /// Est-ce qu'on est arrivé à la position ? On reset
    if (navigationState == NAVIGATION_TARGET_STATE::REACHED)
    {
        const MazeSquare *msq = gladiator->maze->getNearestSquare();
        gladiator->log("On est safe en : %d:%d", msq->i, msq->j);
        gladiator->log("La position safe c'est : %d:%d", savePosition.getX(), savePosition.getY());

        hasToGoToSavePosition = false;
        state = STATE::BASIC;
        resetBasicStrategy();
    }
};

void StateStrategy::useRocketStrategy()
{
    RobotData closestEnnemy = robotsData->getClosestEnnemy();

    gladiator->log("ENNEMY LE PLUS PROCHE : %f:%f", closestEnnemy.position.x, closestEnnemy.position.y);
    /// On récupère l'angle
    float angleRadian = calculerAngle(gladiator->robot->getData().position, closestEnnemy.position);
    /// Se tourne vers la cible

    if (navigation->face(angleRadian))
    {
        /// Lance la roquette
        gladiator->weapon->launchRocket();
        state = STATE::BASIC;
    }
};

bool StateStrategy::shouldLaunchRocket(MyPosition nextPosition)
{
    if (gladiator->weapon->canLaunchRocket())
    {
        /// Si on a déjà une roquette et qu'on va en récupérer une => On launch
        if (gladiator->maze->getSquare(nextPosition.getX(), nextPosition.getY())->coin.value > 0)
        {
            return true;
        }
        /// Si on est près d'un ennemie (simple ?) on launch
        if (robotsData->isEnemyClose(gladiator->maze->getSquareSize() * 5))
        {
            return true;
        }
    }

    return false;
}

bool StateStrategy::shouldSaveMyAss()
{
    const MazeSquare *msq = gladiator->maze->getNearestSquare();
    /// Est-ce qu'on va être en dehors des limites ?
    /// -2 parce qu'on fait comme si y'avait
    int nextMazeHeight = (*mazeHeight - 2);
    int nextMazeLength = (*mazeLength - 2);

    return checkIfIsOutside(msq->i, msq->j, originalMazeHeight, originalMazeLength, &nextMazeHeight, &nextMazeLength);
}