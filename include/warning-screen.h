#ifndef WARNING_SCREEN_H
#define WARNING_SCREEN_H

#include "ticker-screen.h"
#include <Magick++.h>

class WarningScreen : public TickerScreen
{
public:
    WarningScreen();

    virtual void Update(double timeDelta) override;
    virtual void Draw(MatrixFrame *nextFrame) override;

private:
    Magick::Image _segmentImage;
    int           _segmentOffset;
};

#endif
