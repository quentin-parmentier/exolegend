#include "NavigationStack.hpp"
#include <iostream>

NavigationStack::NavigationStack(int stackSize) : stackSize(stackSize), currentIndex(-1)
{
    positions = (MyPosition **)malloc(sizeof(MyPosition *) * stackSize);
    for (int i = 0; i < stackSize; i++)
    {
        positions[i] = new MyPosition();
    }
}

int NavigationStack::spaceLeft()
{
    return stackSize - 1 - currentIndex;
}

bool NavigationStack::hasNext()
{
    return currentIndex > -1;
}

MyPosition NavigationStack::shift()
{
    MyPosition p = *positions[0];
    for (int i = 0; i < currentIndex; i++)
    {
        positions[i] = positions[i + 1];
    }
    currentIndex--;

    return p;
}

void NavigationStack::push(MyPosition pos)
{
    currentIndex++;
    std::cout << "Ajoute une case -" << currentIndex << " POS :" << pos.getX() << ":" << pos.getY() << "\n";
    positions[currentIndex]->copy(pos);
}

void NavigationStack::reset()
{
    currentIndex = -1;
}

MyPosition *NavigationStack::getPositionOnTop()
{
    // std::cout << "CURRENT INDEX :" << currentIndex << "\n";
    // std::cout << "TOP POSITION :" << positions[currentIndex]->getX() << ":" << positions[currentIndex]->getY() << "\n";
    return positions[currentIndex];
}