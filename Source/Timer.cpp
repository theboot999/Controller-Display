#include "Timer.h"


Timer::Timer(const unsigned long& timeout)
{
    period = timeout;
    timeBench = millis();
}

void Timer::SetNewTimeout(const unsigned long& timeout)
{
    period = timeout;
    Reset();
}

void Timer::Reset()
{
    unsigned long currentTime = millis();

    // Handle overflow
    if (currentTime < timeBench)
    {
        timeDiff = (UL_MAX - timeBench) + currentTime;
    }
    else
    {
        timeDiff = currentTime - timeBench;
    }
    timeBench += timeDiff;
}

bool Timer::Fire()
{
    unsigned long currentTime = millis();

    // Handle overflow
    if (currentTime < timeBench)
    {
        timeDiff = (UL_MAX - timeBench) + currentTime;
    }
    else
    {
        timeDiff = currentTime - timeBench;
    }

    if (timeDiff >= period)
    {
        timeBench += timeDiff;
        return true;
    }
    return false;
}