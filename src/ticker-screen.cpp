#include "ticker-screen.h"

#include <time.h>
#include <iostream>
#include <chrono>

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
    _beginTime = chrono::system_clock::now();
}

double TickerScreen::TimeDeltaSeconds() const
{
    auto now = chrono::system_clock::now();
    chrono::duration<double> diff = now - _beginTime;

    return diff.count();
}

bool TickerScreen::running()
{
    return _running && ((duration() < 0) || (TimeDeltaSeconds() < duration()));
}
