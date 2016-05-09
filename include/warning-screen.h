#ifndef WARNING_SCREEN_H
#define WARNING_SCREEN_H

#include "ticker-screen.h"
#include <Magick++.h>

class WarningScreen : public TickerScreen
{
public:
    WarningScreen(Matrix *m);
    virtual void Run() override;

private:
    Magick::Image _segmentImage;
};

#endif
