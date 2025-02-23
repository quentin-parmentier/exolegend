#include "RobotsData.hpp"
#include "IRobotDataFactory.hpp"
#include <memory>
#include "IRobotData.hpp"

class RealRobotData : public IRobotData
{
public:
    RobotData getData() override
    {
        // return gladiator->robot->getData();
    };

    RobotData getData(double x, double y, byte id, byte teamID) override
    {
        Position pos = {x, y, 0};
        return {pos, 0, id, teamID};
    };
};

RobotsData::RobotsData()
{
}

RobotsData::RobotsData(Gladiator *gladiator) : gladiator(gladiator)
{
    // me = gladiator->robot->getData();
    // gladiator->log("Robot id=%d",me.id);
    // for (uint8_t id : gladiator->game->getPlayingRobotsId().ids)
    // {
    //     gladiator->log("Robot id=%d",id);
    //     int advers = 0;
    //     RobotData ret{};
    //     if (id != me.id)
    //     {
    //         gladiator->log("Robot id=%d",id);
    //         // on prend celui dont l'id est différent du robot actuel
    //         ret = gladiator->game->getOtherRobotData(id); // 4.3.3
    //         if(ret.teamId != me.teamId){
    //             if(advers == 0) comp1=ret;
    //             else comp2=ret;
    //             advers++;

    //         }else{
    //             coop = ret;
    //         }
    //     }
    // }
}

void RobotsData::init()
{
    currentRobotID = gladiator->robot->getData().id;

    int advers = 0;
    enemy1ID = 0;
    enemy2ID = 0;

    // gladiator->log("Robot id=%d", me.id);
    for (uint8_t id : gladiator->game->getPlayingRobotsId().ids)
    {
        RobotData robotData{};
        if (id != currentRobotID)
        {
            // on prend celui dont l'id est différent du robot actuel
            robotData = gladiator->game->getOtherRobotData(id); // 4.3.3
            if (robotData.teamId != gladiator->robot->getData().teamId)
            {
                if (advers == 0)
                    enemy1ID = robotData.id;
                else
                    enemy2ID = robotData.id;
                advers++;
            }
            else
            {
                teammateRobotID = robotData.id;
            }
        }
    }
}

bool RobotsData::isEnemyClose(float range)
{
    return isEnemyClose(range, gladiator->robot->getData(), gladiator->game->getOtherRobotData(enemy1ID), gladiator->game->getOtherRobotData(enemy2ID));
}

bool RobotsData::isEnemyClose(float range, RobotData currentRobotData, RobotData enemy1IDData, RobotData enemy2IDData)
{
    /// enemy 1
    if (enemy1ID != 0 && enemy1IDData.lifes > 0 && sqrt(sq(enemy1IDData.position.x - currentRobotData.position.x) + sq(enemy1IDData.position.y - currentRobotData.position.y)) < range)
    {
        // gladiator->log("enemy %d in range", enemy1ID);
        return true;
        // }else if(comp2ID !=8 && comp2.lifes > 0 && distance(MyPosition(comp1.position.x, comp1.position.y), MyPosition(me.position.x, me.position.y) ) < range){
    }
    else if (enemy2ID != 0 && enemy2IDData.lifes > 0 && sqrt(sq(enemy2IDData.position.x - currentRobotData.position.x) + sq(enemy2IDData.position.y - currentRobotData.position.y)) < range)
    {
        // gladiator->log("enemy %d in range", enemy2ID);
        return true;
    }
    else
    {
        return false;
    }
}

bool RobotsData::isAllyClose(float range)
{
    //     RobotData myData = gladiator->robot->getData();
    //     RobotData allyData = currentRobotID == myData.id ? gladiator->game->getOtherRobotData(teammateRobotID) : gladiator->game->getOtherRobotData(currentRobotID);

    //     if (distanceFromPosition(allyData.position, myData.position) < range)
    //     {
    //         //gladiator->log("enemy %d in range", allyData.id);
    //         return true;
    //     }
    //     else
    //     {
    //         return false;
    //     }
    return isAllyClose(range, gladiator->robot->getData(), gladiator->game->getOtherRobotData(teammateRobotID));
}

bool RobotsData::isAllyClose(float range, RobotData currentRobotData, RobotData allyData)
{

    if (distanceFromPosition(allyData.position, currentRobotData.position) < range)
    {
        // gladiator->log("enemy %d in range", allyData.id);
        return true;
    }
    else
    {
        return false;
    }
}

RobotData RobotsData::getClosestAlly()
{
    RobotData myData = gladiator->robot->getData();
    RobotData allyData = currentRobotID == myData.id ? gladiator->game->getOtherRobotData(teammateRobotID) : gladiator->game->getOtherRobotData(currentRobotID);

    return allyData;
}

RobotData RobotsData::getClosestEnemy()
{
    RobotData enemyRobotData1 = gladiator->game->getOtherRobotData(enemy1ID);
    RobotData enemyRobotData2 = gladiator->game->getOtherRobotData(enemy2ID);

    return getClosestRobotData(enemyRobotData1, enemyRobotData2, gladiator->robot->getData().position);
}