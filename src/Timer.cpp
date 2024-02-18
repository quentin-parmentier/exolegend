#include "Timer.hpp"

Timer::Timer(int timeToWait) : lastTime(std::chrono::steady_clock::now()), timeToWait(timeToWait) {}

bool Timer::hasElapsed()
{
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastTime).count();
    return elapsedTime >= timeToWait;
}

bool Timer::mightSaveHisAss()
{
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastTime).count();
    return elapsedTime >= TIME_FOR_SAVE;
}

bool Timer::enemyUpdate()
{
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
    return elapsedTime >= TIME_FOR_ENEMY_UPDATE;
}

void Timer::reset()
{
    lastTime = std::chrono::steady_clock::now();
}