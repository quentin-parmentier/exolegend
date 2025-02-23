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
     * @brief Construct a new RobotsData object without data (for unit test)
     *
     * @param
     */
    RobotsData();

    /**
     * @brief Construct a new RobotsData  object
     *
     * @param gladiator
     */
    RobotsData(Gladiator *gladiator);

    /**
     * @brief Update Data about the different actors
     * @param range Rayon détection en metre
     */
    bool isEnemyClose(float range);
    /**
     * @brief Update Data about the different actors
     * @param range Rayon détection en metre
     */
    bool isEnemyClose(float range, RobotData currentRobot, RobotData enemy1, RobotData enemy2);
    /**
     * @brief Update Data about the different actors
     * @param range Rayon détection en metre
     */
    bool isAllyClose(float range);
    /**
     * @brief Update Data about the different actors
     * @param range Rayon détection en metre
     */
    bool isAllyClose(float range, RobotData currentRobot, RobotData ally);
    RobotData getClosestEnemy();
    RobotData getClosestAlly();
    void init();
    byte currentRobotID;
    byte teammateRobotID;
    byte enemy1ID;
    byte enemy2ID;

private:
    Gladiator *gladiator;
};
#endif