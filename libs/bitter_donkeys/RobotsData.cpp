#include "RobotsData.hpp"

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
    ally1 = gladiator->robot->getData().id;

    int advers = 0;
    ennemy1 = 0;
    ennemy2 = 0;
    
    // gladiator->log("Robot id=%d", me.id);
    for (uint8_t id : gladiator->game->getPlayingRobotsId().ids)
    {
        RobotData robotData{};
        if (id != ally1)
        {
            // on prend celui dont l'id est différent du robot actuel
            robotData = gladiator->game->getOtherRobotData(id); // 4.3.3
            if (robotData.teamId != gladiator->robot->getData().teamId)
            {
                if (advers == 0)
                    ennemy1 = robotData.id;
                else
                    ennemy2 = robotData.id;
                advers++;
            }
            else
            {
                ally2 = robotData.id;
            }
        }
    }
}

bool RobotsData::isEnemyClose(float range)
{
    /// enemy 1
    if (ennemy1 != 0 && gladiator->game->getOtherRobotData(ennemy1).lifes > 0 && sqrt(sq(gladiator->game->getOtherRobotData(ennemy1).position.x - gladiator->robot->getData().position.x) + sq(gladiator->game->getOtherRobotData(ennemy1).position.y - gladiator->robot->getData().position.y)) < range)
    {
        gladiator->log("enemy %d in range", ennemy1);
        return true;
        // }else if(comp2ID !=8 && comp2.lifes > 0 && distance(MyPosition(comp1.position.x, comp1.position.y), MyPosition(me.position.x, me.position.y) ) < range){
    }
    else if (ennemy2 != 0 && gladiator->game->getOtherRobotData(ennemy2).lifes > 0 && sqrt(sq(gladiator->game->getOtherRobotData(ennemy2).position.x - gladiator->robot->getData().position.x) + sq(gladiator->game->getOtherRobotData(ennemy2).position.y - gladiator->robot->getData().position.y)) < range)
    {
        gladiator->log("enemy %d in range", ennemy2);
        return true;
    }
    else
    {
        return false;
    }
}

bool RobotsData::isAllyClose(float range)
{
    RobotData myData = gladiator->robot->getData();
    RobotData allyData = ally1 == myData.id ? gladiator->game->getOtherRobotData(ally2) : gladiator->game->getOtherRobotData(ally1);

    if (distanceFromPosition(allyData.position, myData.position) < range)
    {
        gladiator->log("enemy %d in range", allyData.id);
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
    RobotData allyData = ally1 == myData.id ? gladiator->game->getOtherRobotData(ally2) : gladiator->game->getOtherRobotData(ally1);

    return allyData;
}

RobotData RobotsData::getClosestEnnemy()
{
    RobotData ennemyRobotData1 = gladiator->game->getOtherRobotData(ennemy1);
    RobotData ennemyRobotData2 = gladiator->game->getOtherRobotData(ennemy2);

    return getClosestRobotData(ennemyRobotData1, ennemyRobotData2, gladiator->robot->getData().position);
}