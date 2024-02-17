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

void RobotsData::init(){
    me = gladiator->robot->getData();
    comp1ID = 8;
    comp2ID = 8;
    gladiator->log("Robot id=%d",me.id);
    for (uint8_t id : gladiator->game->getPlayingRobotsId().ids)
    {
        gladiator->log("Robot id=%d",id);
        int advers = 0;
        RobotData ret{};
        if (id != me.id)
        {
            gladiator->log("Robot id=%d",id);
            // on prend celui dont l'id est différent du robot actuel
            ret = gladiator->game->getOtherRobotData(id); // 4.3.3
            if(ret.teamId != me.teamId){
                if(advers == 0) comp1ID=ret.id;
                else comp2ID=ret.id;
                advers++;

            }else{
                coopID = ret.id;
            }
        }
    }
}

void RobotsData::updateOtherData()
{
    if(comp1ID) comp1 = gladiator->game->getOtherRobotData(comp1ID);    
    // std::cout << "comp" << comp1.id <<"\n";
    // gladiator->log(comp1.id);
    if(comp2ID) comp2 = gladiator->game->getOtherRobotData(comp2ID);
    if(coopID) coop = gladiator->game->getOtherRobotData(coopID);
}

RobotsData::isEnemyClose(float range){
    /// enemy 1
    if(comp1ID != 8 && comp1.lifes> 0 && sqrt( sq( comp1.position.x - me.position.x ) + sq( comp1.position.y - me.position.y ) ) < range ){
        return true;
    }else if(comp2ID !=8 && comp2.lifes && distance(MyPosition(comp1.position.x, comp1.position.y), MyPosition(me.position.x, me.position.y) ) < range){ 
        return true;
     }else{
        return false;
     }
}