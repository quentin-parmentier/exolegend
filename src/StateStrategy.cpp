#include "StateStrategy.hpp"
#include "MyPosition.hpp"

StateStrategy::StateStrategy(STATE baseState, Navigation *navigation, NavigationStack *navigationStack, Gladiator *gladiator, NavigationStrategy *navigationStrategy) : state(baseState), navigation(navigation),
                                                                                                                                                                        navigationStack(navigationStack),
                                                                                                                                                                        gladiator(gladiator), navigationStrategy(navigationStrategy)
{
    actualPositionToFind = MyPosition();
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

void StateStrategy::next()
{
    /// On fait les différents checks

    /// On fait l'action en fonction de l'état
    if (state == STATE::BASIC)
    {
        useBasicStrategy();
    }
    else if (state == STATE::SAVE)
    {
    }
    else if (state == STATE::ROCKET)
    {
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
        actualPositionToFind = navigationStack->shift();
        MyPosition *positionOnTop = navigationStack->getPositionOnTop();
        gladiator->log("Nouvelle target %d:%d", positionOnTop->getX(), positionOnTop->getY());
        navigationStrategy->computeRandomPathing(*positionOnTop);
    }
};

void StateStrategy::useSaveStrategy(){

};

void StateStrategy::useRocketStrategy(){

};
