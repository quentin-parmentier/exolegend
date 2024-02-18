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
    state = STATE::BASIC;
}

void StateStrategy::next(bool mazeWillShrink)
{
    /// On fait les différents checks
    if (mazeWillShrink && state != STATE::SAVE)
    {
        gladiator->log("On doit se sauver");
        state = STATE::SAVE;
    }
    else if (state == STATE::SAVE)
    {
        gladiator->log("On doit se sauver");
        state = STATE::SAVE;
    }
    else if (shouldLaunchRocket(actualPositionToFind))
    {
        state = STATE::ROCKET;
    }
    // else if (robotsData->isAllyClose(0.4))
    //{
    //     state = STATE::ESQUIVE;
    // }
    else if (robotsData->isEnemyClose(0.4))
    {
        state = STATE::DEFEND;
    }
    else if (state != STATE::BASIC)
    {
        gladiator->log("On revient à basic");
        state = STATE::BASIC;
        resetBasicStrategy();
    }
    // else if (robotsData->isEnemyClose(0.4))
    //{
    //     // gladiator->log("defend");
    //     state = STATE::DEFEND;
    // }
    // else if (state != STATE::BASIC)
    //{
    //    gladiator->log("On est sauvé");
    //    resetBasicStrategy();
    //    state = STATE::BASIC;
    //}

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
    else if (state == STATE::ESQUIVE)
    {
        useEsquiveStrategy();
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
        if (navigationStack->hasNext())
        {
            actualPositionToFind = navigationStack->shift();
        }
        navigationStrategy->computeRandomPathing(actualPositionToFind);
    }
};

void StateStrategy::useSpinStrategy()
{
    navigation->spin();
}

void StateStrategy::useEsquiveStrategy()
{
    float monX = gladiator->robot->getData().position.x;
    float allyX = robotsData->getClosestAlly().position.x;

    float milieu = gladiator->maze->getSquareSize() * 6;
    if (monX > allyX)
    {
        navigation->driveTo(Vector2(milieu + gladiator->maze->getSquareSize(), milieu));
    }
    else
    {
        navigation->driveTo(Vector2(milieu - gladiator->maze->getSquareSize(), milieu));
    }
}

void StateStrategy::useSaveStrategy()
{
    /// On fonce vers le milieu tant que notre position n'est pas safe
    float milieu = gladiator->maze->getSquareSize() * 6;
    navigation->driveTo(Vector2(milieu, milieu));

    /// Reshrink donc c'est cool
    if (!shouldSaveMyAss())
    {
        state = STATE::BASIC;
        resetBasicStrategy();
    }
};

void StateStrategy::useRocketStrategy()
{
    RobotData closestEnnemy = robotsData->getClosestEnnemy();

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