#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <chrono>

const int TIME_FOR_NEW_WALL = 20;
const int TIME_SHRINK_ALERT = 18;

class Timer
{
private:
    std::chrono::steady_clock::time_point lastTime;

public:
    Timer();
    bool hasElapsed();
    bool mazeWillShrink();
    void reset();
    bool shrinkAlarmTriggered;
};

#endif