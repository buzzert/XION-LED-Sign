#ifndef CLOCK_SCREEN_H
#define CLOCK_SCREEN_H

#include "ticker-screen.h"

#include <string>

class ClockScreen : public TickerScreen
{
public:
    ClockScreen(Matrix *m);
    virtual void Run() override;

private:
    std::string _timeString;
    void _UpdateTimeString();
};

#endif
