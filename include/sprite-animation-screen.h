#ifndef SPRITE_ANIMATION_SCREEN_H
#define SPRITE_ANIMATION_SCREEN_H

#include "ticker-screen.h"
#include "matrix-type.h"
#include <Magick++.h>

#ifdef __linux__
#include <memory>
#endif

class SpriteAnimationScreen : public TickerScreen
{
public:
    SpriteAnimationScreen(Utils::Size size, std::string animationFilePath);

    void Update(double timeDelta) override;
    void Draw(MatrixFrame *nextFrame) override;

private:
    class SpriteAnimationFrame
    {
    public:
        SpriteAnimationFrame(MatrixFrame *frame, int delay) : frame(frame), delay(delay) {};
        int          delay;
        MatrixFrame *frame;
    };

    int _currentFrameIndex = 0;
    std::vector<Magick::Image> _rawImages;

    Magick::Image _GetCurrentImage() const;
    std::vector<std::unique_ptr<SpriteAnimationScreen::SpriteAnimationFrame>> _PreloadedMatrixFrames(Matrix *m);
};

#endif
