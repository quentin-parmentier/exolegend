#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <chrono>

const int TIME_FOR_NEW_WALL = 20;     // seconds
const int TIME_FOR_SAVE = 18;         // seconds
const int TIME_FOR_ENEMY_UPDATE = 50; // millisecs

class Timer
{
private:
    std::chrono::steady_clock::time_point lastTime;

public:
    Timer();
    bool hasElapsed();
    bool mightSaveHisAss();
    bool enemyUpdate();
    void reset();
};

#endif