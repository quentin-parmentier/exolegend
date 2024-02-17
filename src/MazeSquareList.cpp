#include "MazeSquareList.hpp"
#include <iostream>

MazeSquareList::MazeSquareList(int stackSize): stackSize(stackSize), currentIndex(-1) {
    liste = (MazeSquare **) malloc(sizeof(MazeSquare*) * stackSize);
}

int MazeSquareList::spaceLeft() {
    return stackSize - 1 - currentIndex; 
}

bool MazeSquareList::hasNext() {
    return currentIndex > -1;
}

MazeSquare* MazeSquareList::shift() {
    MazeSquare* ms = liste[0];

    for (int i=0; i<currentIndex; i++) {
        liste[i] = liste[i+1];
    }
    currentIndex--;

    return ms;
}

void MazeSquareList::push(MazeSquare* ms) {
    currentIndex++;
    liste[currentIndex] = ms;
}

void MazeSquareList::simplify() {
    // TODO
    // for
    // liste[currentIndex] = ms;
}

void MazeSquareList::reset() {
    currentIndex = -1;
}

void MazeSquareList::printDebug() {
    std::cout << "Size : " << stackSize << " ; currentIndex : " << currentIndex << "\n";
    for (int i=0; i<currentIndex; i++) {
        MazeSquare* ms = liste[i];
        std::cout << "   " << i << " -> i " << ((int) ms->i) << " j " << ((int) ms->j) << "\n";
    }
}