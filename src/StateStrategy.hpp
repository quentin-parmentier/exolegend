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
    StateStrategy(STATE baseState, Navigation *navigation, NavigationStack *navigationStack, Gladiator *gladiator, NavigationStrategy *navigationStrategy);
    void useBasicStrategy();
    void useSaveStrategy();
    void useRocketStrategy();
    void next();
    void resetBasicStrategy();
    MyPosition actualPositionToFind;

private:
    STATE state;
    Navigation *navigation;
    NavigationStack *navigationStack;
    Gladiator *gladiator;
    NavigationStrategy *navigationStrategy;
};

#endif