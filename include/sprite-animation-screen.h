#ifndef SPRITE_ANIMATION_SCREEN_H
#define SPRITE_ANIMATION_SCREEN_H

#include "ticker-screen.h"
#include "matrix-type.h"
#include <Magick++.h>

class SpriteAnimationScreen : public TickerScreen
{
public:
    SpriteAnimationScreen(Matrix *m, std::string animationFilePath);
    void Run() override;

private:
    class SpriteAnimationFrame;
    
    std::vector<Magick::Image> _rawImages;
    std::vector<SpriteAnimationFrame> _frames;
};

#endif
