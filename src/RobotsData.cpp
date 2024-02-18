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
    
    coop[0] = gladiator->robot->getData().id;

    // gladiator->log("Robot id=%d", me.id);
    for (uint8_t id : gladiator->game->getPlayingRobotsId().ids)
    {
        int advers = 0;
        comp[0] = 0;
        comp[1] =0;
        coop[0]=0;
        RobotData ret{};
        if (id != coop[0])
        {
            // on prend celui dont l'id est différent du robot actuel
            ret = gladiator->game->getOtherRobotData(id); // 4.3.3
            if (ret.teamId != gladiator->robot->getData().teamId)
            {
                if (advers == 0)
                    comp[0] = ret.id;
                else
                    comp[1] = ret.id;
                advers++;
            }
            else
            {
                coop[1] = ret.id;
            }
        }
    }
}


RobotsData::isEnemyClose(float range)
{
    /// enemy 1
    if (comp[0] !=0 && gladiator->game->getOtherRobotData(comp[0]).lifes > 0 && sqrt(sq(gladiator->game->getOtherRobotData(comp[0]).position.x - gladiator->robot->getData().position.x) + sq(gladiator->game->getOtherRobotData(comp[0]).position.y - gladiator->robot->getData().position.y)) < range)
    {
        gladiator->log("enemy %d in range", comp[0]);
        return true;
        // }else if(comp2ID !=8 && comp2.lifes > 0 && distance(MyPosition(comp1.position.x, comp1.position.y), MyPosition(me.position.x, me.position.y) ) < range){
    }
    else if (comp[1] != 0 && gladiator->game->getOtherRobotData(comp[1]).lifes > 0 && sqrt(sq(gladiator->game->getOtherRobotData(comp[1]).position.x - gladiator->robot->getData().position.x) + sq(gladiator->game->getOtherRobotData(comp[1]).position.y - gladiator->robot->getData().position.y))  < range)
    {
        gladiator->log("enemy %d in range", comp[1]);
        return true;
    }
    else
    {
        return false;
    }
}