#include "NavigationStack.hpp"

NavigationStack::NavigationStack(int stackSize): stackSize(stackSize), currentIndex(-1) {
    positions = (MyPosition **) malloc(sizeof(MyPosition*) * stackSize);
    for (int i=0; i<stackSize; i++){
        positions[i] = new MyPosition();
    }
}

int NavigationStack::spaceLeft() {
    return stackSize - 1 - currentIndex; 
}

bool NavigationStack::hasNext() {
    return currentIndex > -1;
}

MyPosition NavigationStack::shift() {
    MyPosition p = *positions[0];

    for (int i=0; i<currentIndex-1; i++) {
        positions[i] = positions[i+1];
    }
    currentIndex--;

    return p;
}

void NavigationStack::push(MyPosition pos) {
    currentIndex++;
    positions[currentIndex]->copy(pos);
}

void NavigationStack::reset() {
    currentIndex = -1;
}