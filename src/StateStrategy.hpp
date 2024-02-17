#ifndef STATE_HPP_
#define STATE_HPP_

#include "Navigation.hpp"
#include "NavigationStack.hpp"
#include "NavigationStrategy.hpp"

enum STATE
{
    BASIC,
    SAVE,
    ROCKET,
};

class StateStrategy
{

public:
    StateStrategy(STATE baseState, Navigation *navigation, NavigationStack *navigationStack, Gladiator *gladiator, NavigationStrategy *navigationStrategy, int originalMazeHeight, int originalMazeLength, int *mazeHeight, int *mazeLength);
    void useBasicStrategy();
    void useSaveStrategy();
    void useRocketStrategy();
    void next(bool mazeWillShrink);
    void resetBasicStrategy();
    MyPosition actualPositionToFind;
    bool shouldSaveMyAss();

private:
    STATE state;
    Navigation *navigation;
    NavigationStack *navigationStack;
    Gladiator *gladiator;
    NavigationStrategy *navigationStrategy;
    int originalMazeHeight;
    int originalMazeLength;
    int *mazeHeight;
    int *mazeLength;
    MyPosition savePostition;
    bool hasToGoToSavePosition;
};

#endif