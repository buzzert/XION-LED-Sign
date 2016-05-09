#include "screen-coordinator.h"
#include <chrono>
#include <iostream>

using namespace std;

#define HZ_30_US (1000000 / 30)

ScreenCoordinator::ScreenCoordinator(Matrix *m, std::vector<TickerScreen *> screens)
    : _screens(screens), _running(true), _matrix(m)
{

}

void ScreenCoordinator::Run()
{
    MatrixFrame *offscreenFrame = _matrix->CreateFrameCanvas();

    int currentScreenIdx = 0;
    while (running())
    {
        TickerScreen *screen = _screens[currentScreenIdx];

        screen->_ResetBeginTime();
        screen->Start();

        while (screen->running()) {
            auto beginUpdateTime = chrono::high_resolution_clock::now();

            // Update
            double timeDelta = screen->_TimeDeltaSeconds();
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

            useconds_t sleepDuration = HZ_30_US - updateTimeUs.count();
            usleep(sleepDuration);
        }

        screen->Stop();

        currentScreenIdx = (currentScreenIdx + 1) % _screens.size();
    }
}
