#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <chrono>

const int TIME_FOR_NEW_WALL = 20;
const int TIME_FOR_SAVE = 18;

class Timer
{
private:
    std::chrono::steady_clock::time_point lastTime;

public:
    Timer();
    bool hasElapsed();
    bool mightSaveHisAss();

    void reset();
};

#endif