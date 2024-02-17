#include "Timer.hpp"

Timer::Timer() : lastTime(std::chrono::steady_clock::now()) {}

bool Timer::hasElapsed()
{
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastTime).count();
    return elapsedTime >= TIME_FOR_NEW_WALL;
}

bool Timer::mightSaveHisAss()
{
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastTime).count();
    return elapsedTime >= TIME_FOR_SAVE;
}

void Timer::reset()
{
    lastTime = std::chrono::steady_clock::now();
}