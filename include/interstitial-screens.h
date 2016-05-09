#ifndef INTERSTITIAL_SCREENS_H
#define INTERSTITIAL_SCREENS_H

#include "ticker-screen.h"
#include "rasterized-frame.h"
#include <Magick++.h>

namespace InterstitialScreen {

class RainbowRoad : public TickerScreen
{
public:
    RainbowRoad(Utils::Size canvasSize) : _canvasSize(canvasSize) {};

    void Start() override;
    void Update(double timeDelta) override;
    void Draw(MatrixFrame *nextFrame) override;

private:
    int           _rainbowOffset;
    Utils::Pixel *_horizontalPixels;
    Utils::Size   _canvasSize;
};

class DDRArrows : public TickerScreen
{
public:
    DDRArrows(Utils::Size canvasSize);

    void Start() override;
    void Stop() override;
    void Update(double timeDelta) override;
    void Draw(MatrixFrame *nextFrame) override;

    Utils::Size canvasSize;

private:
    enum ArrowColor { Blue, Pink };
    struct Arrow {
        Arrow() : position(0, 0) {};
        Utils::Point<double> position;
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
