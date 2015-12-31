#ifndef SCREEN_COORDINATOR_H
#define SCREEN_COORDINATOR_H

#include "thread.h"
#include "ticker-screen.h"

class ScreenCoordinator : public rgb_matrix::Thread
{
public:
    ScreenCoordinator(std::vector<TickerScreen *> screens);

    virtual void Run() override;
    bool running() { return _running; }

private:
    bool _running = true;
    std::vector<TickerScreen *> _screens;
};

#endif
