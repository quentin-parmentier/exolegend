#ifndef ROBOTSDATA_HPP_
#define ROBOTSDATA_HPP_

#include <gladiator.h>
#include <cmath>

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
    updateOtherData();
    /**
     * @brief Update Data about the different actors
     *
     */
    isEnemyClose(float range);
    RobotData me;
    RobotData coop;
    RobotData comp1;
    RobotData comp2;

private:
    Gladiator *gladiator;
};
#endif