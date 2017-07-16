#include "screen-coordinator.h"
#include <chrono>
#include <iostream>

using namespace std;

#define HZ_30_US (1000000 / 30)

ScreenCoordinator::ScreenCoordinator(Matrix *m, std::vector<TickerScreen *> screens)
    : _screens(screens), _running(true), _matrix(m)
{

}

TickerScreen* ScreenCoordinator::_NextScreen()
{
    TickerScreen *nextScreen = nullptr;
    if (_interludeScreen && !_showingInterlude) {
        // return interlude screen
        nextScreen = _interludeScreen;
        
        _showingInterlude = true;
    } else {
        nextScreen = _screens[_currentScreenIdx];
        _currentScreenIdx = (_currentScreenIdx + 1) % _screens.size();

        _showingInterlude = false;
    }

    return nextScreen;
}

void ScreenCoordinator::Run()
{
    MatrixFrame *offscreenFrame = _matrix->CreateFrameCanvas();

    while (running())
    {
        TickerScreen *screen = _NextScreen();

        screen->ResetBeginTime();
        screen->Start();

        while (screen->running()) {
            auto beginUpdateTime = chrono::high_resolution_clock::now();

            // Update
            double timeDelta = screen->TimeDeltaSeconds();
            screen->Update(timeDelta);

            // Prepare next frame (double buffering)
            MatrixFrame *nextFrame = offscreenFrame;
            nextFrame->Clear();

            // Draw
            screen->Draw(nextFrame);

            // Swap
            offscreenFrame = _matrix->SwapOnVSync(nextFrame);

            auto endUpdateTime = chrono::high_resolution_clock::now();
            auto updateTimeUs = chrono::duration_cast<chrono::microseconds>(endUpdateTime - beginUpdateTime);

            useconds_t sleepDuration = HZ_30_US;
            if (updateTimeUs.count() < sleepDuration) {
                sleepDuration = sleepDuration - updateTimeUs.count();
            }

            usleep(sleepDuration);
        }

        screen->Stop();
    }
}
