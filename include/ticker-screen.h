#ifndef TICKER_SCREEN_H
#define TICKER_SCREEN_H

#include "threaded-canvas-manipulator.h"
#include "matrix-type.h"
#include <unistd.h>

class TickerScreen
{
public:
    TickerScreen(Utils::Size canvasSize);
    virtual ~TickerScreen();

    virtual void Start() { _running = true; };
    virtual void Stop() { _running = false; };

    // Update actors
    virtual void Update(double timeDelta) = 0;

    // Draw into nextFrame
    virtual void Draw(MatrixFrame *nextFrame) = 0;

    virtual double duration() const { return _duration; };
    void setDuration(double seconds) { _duration = seconds; }
        // set to a negative number to run forever.

    bool running();

    Utils::Size canvasSize;

    void   ResetBeginTime();
    double TimeDeltaSeconds() const;

protected:
    bool   _running = true;
    double _duration = 8; // default in seconds

    std::chrono::system_clock::time_point _beginTime;
};

#endif
