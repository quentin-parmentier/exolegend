#include "StateStrategy.hpp"
#include "MyPosition.hpp"
#include "RobotsData.hpp"
#include "utils.hpp"

StateStrategy::StateStrategy(STATE baseState, RobotsData *robotsData, Navigation *navigation, NavigationStack *navigationStack, Gladiator *gladiator, NavigationStrategy *navigationStrategy, int originalMazeHeight, int originalMazeLength, int *mazeHeight, int *mazeLength) : state(baseState), robotsData(robotsData), navigation(navigation),
                                                                                                                                                                                                                                                          navigationStack(navigationStack),
                                                                                                                                                                                                                                                          gladiator(gladiator), navigationStrategy(navigationStrategy), mazeHeight(mazeHeight),
                                                                                                                                                                                                                                                          mazeLength(mazeLength), originalMazeHeight(originalMazeHeight),
                                                                                                                                                                                                                                                          originalMazeLength(originalMazeLength)
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

    // state = STATE::BASIC; ?? @todo
}

void StateStrategy::next(bool mazeWillShrink)
{
    /// On fait les différents checks
    if (mazeWillShrink && shouldSaveMyAss())
    {
        state = STATE::SAVE;
    }else if(robotsData->isEnemyClose(0.4)){
        state = STATE::DEFEND;
    }

    /// On fait l'action en fonction de l'état
    if (state == STATE::BASIC)
    {
        useBasicStrategy();
    }
    else if (state == STATE::DEFEND)
    {
        // ajouter toupie 
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
    /// On trouve la case la plus proche

    /// On y va à pleine balle tout droit
};

void StateStrategy::useRocketStrategy(){
    if(gladiator->weapon->canLaunchRocket()){
        gladiator->weapon->launchRocket();
    }
};

bool StateStrategy::shouldSaveMyAss()
{
    const MazeSquare *msq = gladiator->maze->getNearestSquare();
    /// Est-ce qu'on va être en dehors des limites ?
    /// -2 parce qu'on fait comme si y'avait
    // int mazeHeight = (*mazeHeight - 2);
    // int mazeLength = (*mazeLength - 2);
    // return checkIfIsOutside(msq->i, msq->j, originalMazeHeight, originalMazeLength, &mazeHeight, &mazeLength);
}