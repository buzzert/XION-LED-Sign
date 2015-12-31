#include "ticker-screen.h"

TickerScreen::TickerScreen(Matrix *m)
    : rgb_matrix::ThreadedCanvasManipulator(m), _matrix(m)
{

}

TickerScreen::~TickerScreen()
{

}
