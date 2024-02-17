#ifndef MAZESQUARELIST_HPP_
#define MAZESQUARELIST_HPP_

#include "gladiator.h"

/**
 * @brief Repréente une file de navigation (FIFO)
 * 
 */
class MazeSquareList {

    public:
        /**
         * @brief Construct a new Navigation Stack object
         * 
         * @param stackSize 
         */
        MazeSquareList(int stackSize);

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
         * @brief Retourne l'élément en tête de liste et le retire de la liste
         * 
         * @return MazeSquare* 
         */
        MazeSquare* shift();

        /**
         * @brief Ajoute un élément en fin de liste
         * 
         * @param pos 
         */
        void push(MazeSquare* ms);

        /**
         * @brief Vide la liste
         * 
         */
        void reset();

        /**
         * @brief affiche du débug
         * 
         */
        void printDebug();

    private:
        MazeSquare** liste;
        int stackSize;
        int currentIndex;

};

#endif