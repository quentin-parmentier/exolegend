#ifndef ROBOTSDATA_HPP_
#define ROBOTSDATA_HPP_

#include <gladiator.h>
#include <cmath>
#include "utils.hpp"

/**
 * @brief Repréente une file de navigation (FIFO)
 *
 */
class RobotsData
{
public:
    /**
     * @brief Construct a new RobotsData  object
     *
     * @param robotList
     * @param teamId
     */
    RobotsData(Gladiator *gladiator);

    /**
     * @brief Update Data about the different actors
     *
     */
    bool isEnemyClose(float range);
    bool isAllyClose(float range);
    RobotData getClosestEnnemy();
    RobotData getClosestAlly();
    void init();
    byte coop[2];
    byte comp[2];

private:
    Gladiator *gladiator;
};
#endif