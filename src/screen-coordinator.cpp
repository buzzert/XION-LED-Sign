#include "screen-coordinator.h"

ScreenCoordinator::ScreenCoordinator(std::vector<TickerScreen *> screens)
    : _screens(screens), _running(true)
{

}

void ScreenCoordinator::Run()
{
    int currentScreenIdx = 0;
    while (running())
    {
        TickerScreen *screen = _screens[currentScreenIdx];
        screen->Start();
        while (screen->running()); // spin lock
        screen->Stop();
        screen->WaitStopped();

        currentScreenIdx = (currentScreenIdx + 1) % _screens.size();
    }
}
