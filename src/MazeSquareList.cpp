#include "MazeSquareList.hpp"

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

    for (int i=0; i<currentIndex-1; i++) {
        liste[i] = liste[i+1];
    }
    currentIndex--;

    return ms;
}

void MazeSquareList::push(MazeSquare* ms) {
    currentIndex++;
    liste[currentIndex] = ms;
}

void MazeSquareList::reset() {
    currentIndex = -1;
}