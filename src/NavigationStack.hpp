#ifndef NAVIGATION_STACK_
#define NAVIGATION_STACK_

#include "MyPosition.hpp"

/**
 * @brief Repréente une file de navigation (FIFO)
 *
 */
class NavigationStack
{

public:
    /**
     * @brief Construct a new Navigation Stack object
     *
     * @param stackSize
     */
    NavigationStack(int stackSize);

    /**
     * @brief Indique la place restante dans la liste
     *
     * @return int
     */
    int spaceLeft();

    /**
     * @brief Indique s'il y a un élément à lire
     *
     * @return true
     * @return false
     */
    bool hasNext();

    /**
     * @brief Retourne l'élément en tête de liste et le retire d ela liste
     *
     * @return MyPosition
     */
    MyPosition shift();

    /**
     * @brief Ajoute un élément en fin de liste
     *
     * @param pos
     */
    void push(MyPosition pos);

    /**
     * @brief Vide la liste
     *
     */
    void reset();

    /**
     * @brief Récupère la position au top de la liste
     */
    MyPosition *getPositionOnTop();

    /**
     * @brief Permet de voir l'ensemble de la liste
     */
    void printTab();

    /**
     * @brief Permet de reduire la liste pour aller plus vite sur les lignes droites 
     */
    void simplify();

private:
    MyPosition **positions;
    MyPosition **positionsTemp;
    int stackSize;
    int currentIndex;
};

#endif