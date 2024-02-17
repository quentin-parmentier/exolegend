#include "RobotsData.hpp"

RobotsData::RobotsData(Gladiator *gladiator) : gladiator(gladiator)
{

    for (uint8_t id : gladiator->game->getPlayingRobotsId().ids)
    {
        int advers = 0;
        me = gladiator->robot->getData();
        RobotData ret{};
        if (id != me.id)
        {
            // on prend celui dont l'id est différent du robot actuel
            ret = gladiator->game->getOtherRobotData(id); // 4.3.3
            if(ret.teamId != me.teamId){
                if(advers == 0) comp1=ret;
                else comp2=ret;
                advers++;

            }else{
                coop = ret;
            }
        }
    }
}

RobotsData::updateOtherData()
{
    comp1 = gladiator->game->getOtherRobotData(comp1.id);
    comp2 = gladiator->game->getOtherRobotData(comp2.id);
    coop = gladiator->game->getOtherRobotData(coop.id);
}

RobotsData::isEnemyClose(float range){
    /// enemy 1
    if( sqrt( sq( comp1.position.x - me.position.x ) + sq( comp1.position.y - me.position.y ) ) < range ){
        return true;
    }else if(sqrt( sq( comp2.position.x - me.position.x ) + sq( comp2.position.y - me.position.y ) ) < range){ 
        return true;
     }else{
        return false;
     }
}