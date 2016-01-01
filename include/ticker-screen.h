#ifndef TICKER_SCREEN_H
#define TICKER_SCREEN_H

#include "threaded-canvas-manipulator.h"
#include "matrix-type.h"
#include <unistd.h>

class TickerScreen : public rgb_matrix::ThreadedCanvasManipulator
{
public:
    TickerScreen(Matrix *m);
    virtual ~TickerScreen();

    virtual void Start(int realtime_priority = 0, uint32_t affinity_mask = 0);
    virtual void Run() = 0;

    virtual int duration() { return _duration; };
    void setDuration(int seconds) { _duration = seconds; }
        // set to a negative number to run forever.

    bool running();

    double refreshRate() { return (1000000 / 60); };
        // convenience. returns refresh rate in microseconds

protected:
    time_t _beginTime;
    int _duration = 8; // default in seconds
    Matrix *const _matrix;

    MatrixFrame *_offscreenFrame;
};

#endif
