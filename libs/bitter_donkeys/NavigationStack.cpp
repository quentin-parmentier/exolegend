#include "NavigationStack.hpp"
#include <iostream>

NavigationStack::NavigationStack(int stackSize) : stackSize(stackSize), currentIndex(-1)
{
    positions = (MyPosition **)malloc(sizeof(MyPosition *) * stackSize);
    positionsTemp = (MyPosition **)malloc(sizeof(MyPosition *) * stackSize);

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
        *positions[i] = *positions[i + 1];
    }
    currentIndex--;

    return p;
}

void NavigationStack::push(MyPosition pos)
{
    currentIndex++;
    std::cout << "Ajoute une case - " << currentIndex << " POS :" << pos.getX() << ":" << pos.getY() << "\n";
    positions[currentIndex]->copy(pos);
}

void NavigationStack::reset()
{
    currentIndex = -1;
}

void NavigationStack::simplify()
{
    std::cout << "start SIpmlify \n";
    *positionsTemp[0] = *positions[0];
    int tableposition = 0;

    // iterate over X
    for (int i = 1; i <= currentIndex - 1; i++)
    {
        // std::cout << "iteration" << i <<"\n";
        // si le point n'a pas le même X que ces voisins
        if ((positions[i - 1]->getX() == positions[i]->getX() && positions[i + 1]->getX() == positions[i]->getX() && abs(positions[i + 1]->getY() - positions[i - 1]->getY()) != 0) || (positions[i - 1]->getY() == positions[i]->getY() && positions[i + 1]->getY() == positions[i]->getY() && abs(positions[i + 1]->getX() - positions[i - 1]->getX()) != 0))
        {
            std::cout << "ignore x " << positions[i]->getX() << " y " << positions[i]->getY() << "\n";
            // tableposition++;
            // positionsTemp[tableposition] = positions[i];
        }
        else
        {
            tableposition++;
            *positionsTemp[tableposition] = *positions[i];
        }
    }
    tableposition++;
    *positionsTemp[tableposition] = *positions[currentIndex];
    currentIndex = tableposition;
    for (int i = 0; i <= currentIndex; i++)
    {
        *positions[i] = *positionsTemp[i];
    }
    std::cout << "end SIpmlify \n";
}

MyPosition *NavigationStack::getPositionOnTop()
{
    // std::cout << "CURRENT INDEX :" << currentIndex << "\n";
    // std::cout << "TOP POSITION :" << positions[currentIndex]->getX() << ":" << positions[currentIndex]->getY() << "\n";
    return positions[currentIndex];
}

void NavigationStack::printTab()
{
    for (int i = 0; i < currentIndex; i++)
    {
        std::cout << " CASE - " << i << " VALEUR : " << positions[i]->getX() << ":" << positions[i]->getY() << "\n";
    }
}