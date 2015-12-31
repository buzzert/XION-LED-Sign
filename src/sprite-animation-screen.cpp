#include "sprite-animation-screen.h"

#include "matrix-type.h"
#include "canvas.h"
#include <iostream>

using namespace rgb_matrix;

SpriteAnimationScreen::SpriteAnimationScreen(Matrix *m, std::string animationFilePath)
    : TickerScreen(m)
{
    // Load image frames
    std::vector<Magick::Image> animationFrames;
    readImages(&animationFrames, animationFilePath);

    if (animationFrames.size() > 1) {
        Magick::coalesceImages(&_rawImages, animationFrames.begin(), animationFrames.end());
    } else {
        _rawImages.push_back(animationFrames[0]);
    }

    // Convert image frames into canvases
    for (int i = 0; i < _rawImages.size(); i++) {
        Magick::Image image = _rawImages[i];
        MatrixFrame *frameCanvas = _matrix->CreateFrameCanvas();

        for (size_t y = 0; y < image.rows(); ++y) {
            for (size_t x = 0; x < image.columns(); ++x) {
                const Magick::Color &c = image.pixelColor(x, y);
                frameCanvas->SetPixel(x, y,
                               ScaleQuantumToChar(c.redQuantum()),
                               ScaleQuantumToChar(c.greenQuantum()),
                               ScaleQuantumToChar(c.blueQuantum())
               );
            }
        }

        SpriteAnimationFrame animFrame(frameCanvas, image.animationDelay());
        _frames.push_back(animFrame);
    }
}

void SpriteAnimationScreen::Run()
{
    int currentFrameIndex = 0;
    while (running()) {
        SpriteAnimationFrame nextFrame = _frames[currentFrameIndex];

        MatrixFrame *frame = nextFrame.frame;
        _matrix->SwapOnVSync(frame);

        currentFrameIndex = (currentFrameIndex + 1) % _frames.size();
        usleep(nextFrame.delay * 10000);
    }
}
