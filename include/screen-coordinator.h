#ifndef SCREEN_COORDINATOR_H
#define SCREEN_COORDINATOR_H

#include "thread.h"
#include "ticker-screen.h"

#include <vector>

class ScreenCoordinator : public rgb_matrix::Thread
{
public:
    ScreenCoordinator(Matrix *m, std::vector<TickerScreen *> screens);

    virtual void Run() override;
    bool running() { return _running; }

    void ScheduleInterludeScreen(TickerScreen *screen) { _interludeScreen = screen; };
        // xxx: this should probably be copied or at least a smart pointer...

private:
    bool _running = true;
    int _currentScreenIdx = 0;

    bool _showingInterlude = false;
    TickerScreen* _interludeScreen = nullptr;

    std::vector<TickerScreen *> _screens;
    Matrix *const _matrix;

    TickerScreen* _NextScreen();
};

#endif
