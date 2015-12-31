#ifndef TICKER_SCREEN_H
#define TICKER_SCREEN_H

#include "threaded-canvas-manipulator.h"
#include "matrix-type.h"

class TickerScreen : public rgb_matrix::ThreadedCanvasManipulator
{
public:
    TickerScreen(Matrix *m);
    virtual ~TickerScreen();

    virtual void Run() = 0;

protected:
    Matrix *const _matrix;
};

#endif
