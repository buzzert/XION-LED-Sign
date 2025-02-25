#include "ticker-screen.h"

#include <time.h>
#include <iostream>

using namespace std;

TickerScreen::TickerScreen(Utils::Size canvasSize)
    : canvasSize(canvasSize)
{
}

TickerScreen::~TickerScreen()
{

}

void TickerScreen::ResetBeginTime()
{
    clock_gettime(CLOCK_MONOTONIC, &_beginTime);
}

double TickerScreen::TimeDeltaSeconds() const
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    
    double seconds = (now.tv_sec - _beginTime.tv_sec) + 
                     (now.tv_nsec - _beginTime.tv_nsec) / 1000000000.0;
    
    return seconds;
}

bool TickerScreen::running()
{
    return _running && ((duration() < 0) || (TimeDeltaSeconds() < duration()));
}
