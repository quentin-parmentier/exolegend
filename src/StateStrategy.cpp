#include "StateStrategy.hpp"
#include "MyPosition.hpp"
#include "utils.hpp"

StateStrategy::StateStrategy(STATE baseState, Navigation *navigation, NavigationStack *navigationStack, Gladiator *gladiator, NavigationStrategy *navigationStrategy, int originalMazeHeight, int originalMazeLength, int *mazeHeight, int *mazeLength) : state(baseState), navigation(navigation),
                                                                                                                                                                                                                                                          navigationStack(navigationStack),
                                                                                                                                                                                                                                                          gladiator(gladiator), navigationStrategy(navigationStrategy), originalMazeHeight(originalMazeHeight),
                                                                                                                                                                                                                                                          originalMazeLength(originalMazeLength), mazeHeight(mazeHeight),
                                                                                                                                                                                                                                                          mazeLength(mazeLength)
{
    actualPositionToFind = MyPosition();
    savePostition = MyPosition();
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

    // state = STATE::BASIC; ?? @todo
}

void StateStrategy::next(bool mazeWillShrink)
{
    /// On fait les différents checks
    if (mazeWillShrink && state != STATE::SAVE && shouldSaveMyAss())
    {
        state = STATE::SAVE;
    }

    /// On fait l'action en fonction de l'état
    if (state == STATE::BASIC)
    {
        useBasicStrategy();
    }
    else if (state == STATE::SAVE)
    {
        useSaveStrategy();
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
        savePostition = getNearestValidPosition(msq->i, msq->j, originalMazeHeight, originalMazeLength, nextMazeHeight, nextMazeLength);

        gladiator->log("On est en : %d:%d", msq->i, msq->j);
        gladiator->log("La position de repli c'est : %d:%d", savePostition.getX(), savePostition.getY());
    }

    /// On y va à pleine balle tout droit
    NAVIGATION_TARGET_STATE navigationState = navigation->driveTo(actualPositionToFind.toVector(gladiator->maze->getSquareSize()));
    /// Est-ce qu'on est arrivé à la position ? On reset
    if (navigationState == NAVIGATION_TARGET_STATE::REACHED)
    {
        hasToGoToSavePosition = false;
        state = STATE::BASIC;
        resetBasicStrategy();
    }
};

void StateStrategy::useRocketStrategy(){

};

bool StateStrategy::shouldSaveMyAss()
{
    const MazeSquare *msq = gladiator->maze->getNearestSquare();
    /// Est-ce qu'on va être en dehors des limites ?
    /// -2 parce qu'on fait comme si y'avait
    int nextMazeHeight = (*mazeHeight - 2);
    int nextMazeLength = (*mazeLength - 2);

    return checkIfIsOutside(msq->i, msq->j, originalMazeHeight, originalMazeLength, &nextMazeHeight, &nextMazeLength);
}