#include "Time.hpp"

Time::Time()
    : previousTime(0.0), deltaTime(0.0), fps(0.0)
{
}

void Time::Update(double currentTime)
{
    deltaTime = currentTime - previousTime;
    previousTime = currentTime;
    if (deltaTime > 0.0f)
    {
        fps = 1.0f / deltaTime;
    }
}

double Time::GetDeltaTime() const
{
    return deltaTime;
}

double Time::GetFPS() const
{
    return fps;
}
