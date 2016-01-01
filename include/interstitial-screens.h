#ifndef INTERSTITIAL_SCREENS_H
#define INTERSTITIAL_SCREENS_H

#include "ticker-screen.h"
#include "rasterized-frame.h"
#include <Magick++.h>

namespace InterstitialScreen {

class RainbowRoad : public TickerScreen
{
public:
    RainbowRoad(Matrix *m) : TickerScreen(m) {};
    void Run() override;
};

class DDRArrows : public TickerScreen
{
public:
    DDRArrows(Matrix *m);
    void Run() override;

private:
    enum ArrowColor { Blue, Pink };
    struct Arrow {
        Arrow() : position(0, 0) {};
        Utils::Point position;
        ArrowColor color;
        int angle;
    };

    Magick::Image _blueArrowImage;
    Magick::Image _pinkArrowImage;

    int _randomArrowIndex = 0;
    std::vector<Arrow> _arrows;
    void _GenerateNewArrowPosition();
};


} // namespace InterstitialScreen

#endif
