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
    void updateOtherData();
    /**
     * @brief Update Data about the different actors
     *
     */
    isEnemyClose(float range);
    void init();
    RobotData me;
    RobotData coop;
    RobotData comp1;
    RobotData comp2;
    byte comp1ID;
    byte comp2ID;
    byte coopID;

private:
    Gladiator *gladiator;
};
#endif