#ifndef XAIBATSU_H
#define XAIBATSU_H

#include "ticker-screen.h"
#include "graphics.h"
#include "rasterized-frame.h"

#include <memory>

class Xaibatsu : public TickerScreen
{
public:
    Xaibatsu(Utils::Size canvasSize);

    void Start() override;
    void Update(double timeDelta) override;
    void Draw(MatrixFrame *m) override;

private:
    std::unique_ptr<RasterizedFrame> _labelLayer;
    std::unique_ptr<RasterizedFrame> _backgroundLayer;
    int _backgroundOffset = 0;

    bool _cursorVisible = false;
    double _cursorBlinkTime = 0;
};

#endif
