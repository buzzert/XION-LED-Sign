#ifndef CLOCK_SCREEN_H
#define CLOCK_SCREEN_H

#include "ticker-screen.h"

#include <string>

class ClockScreen : public TickerScreen
{
public:
    ClockScreen();

    void Update(double timeDelta) override;
    void Draw(MatrixFrame *nextFrame) override;

private:
    std::string _timeString;
    void _UpdateTimeString();
};

#endif
