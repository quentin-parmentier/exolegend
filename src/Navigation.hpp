#ifndef NAVIGATION_HPP_
#define NAVIGATION_HPP_

#include <gladiator.h>
#include "Vector2.hpp"
#include "utils.hpp"

enum class NAVIGATION_TARGET_STATE {
    NOT_SET,
    REACHED,
    FAR,
    CLOSE,
};

class Navigation {

    public:
        Navigation(Gladiator *gladiator);
        NAVIGATION_TARGET_STATE driveTo(const Vector2 &target, bool showLogs = false);

        /**
         * @brief Fait tourner rapidement le robot sur lui même
         */
        void spin();

    private:
        /**
         * @brief référence au robot
         * 
         */
        Gladiator *gladiator;

        /**
         * @brief Position de départ du robot
         * 
         */
        Vector2 startPosition;

        /**
         * @brief Compteur de rotation pour détecter qu'on arrive pas à atteindre notre point d'arrivée
         * 
         */
        float rotationCounter;

        /**
         * @brief Dernière position connue
         * 
         */
        Vector2 previousPosition;

        /**
         * @brief Dernière cible vers laquelle on se dirige
         * 
         */
        Vector2 previousTargetPosition;

        /**
         * @brief Dernier état de navigation
         * 
         */
        NAVIGATION_TARGET_STATE lastState;

        /**
         * @brief Calcule la vitesse des moteurs en marche avant
         * 
         * @param distanceFromStart 
         * @param distanceFromTarget 
         * @return float 
         */
        inline float speedValueFromDistance(float distanceFromStart, float distanceFromTarget);

        /**
         * @brief Calcule la correction d'angle à appliquer au robot par rapport à l'angle à atteindre
         * 
         * @param angleOffset 
         * @return float 
         */
        inline float rotationOffsetFromAngleError(float angleOffset);
};

#endif