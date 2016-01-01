#include "ticker-screen.h"

#include <time.h>
#include <iostream>

TickerScreen::TickerScreen(Matrix *m)
    : rgb_matrix::ThreadedCanvasManipulator(m), _matrix(m)
{
    _offscreenFrame = m->CreateFrameCanvas();
}

TickerScreen::~TickerScreen()
{

}

void TickerScreen::Start(int realtime_priority, uint32_t affinity_mask)
{
    ThreadedCanvasManipulator::Start(realtime_priority, affinity_mask);
    time(&_beginTime);
}

bool TickerScreen::running()
{
    if (duration() > 0) {
        time_t now;
        time(&now);

        double secondsElapsed = difftime(now, _beginTime);
        if (secondsElapsed > duration()) {
            Stop();
        }
    }

    return ThreadedCanvasManipulator::running();
}
