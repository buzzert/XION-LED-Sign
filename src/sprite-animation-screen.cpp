#include "sprite-animation-screen.h"

#include "matrix-type.h"
#include "canvas.h"
#include <sys/stat.h>
#include <iostream>

using namespace rgb_matrix;
using namespace std;

static bool _fileExists(std::string filepath)
{
    struct stat buffer;
    return ( stat(filepath.c_str(), &buffer) == 0 );
}

SpriteAnimationScreen::SpriteAnimationScreen(Matrix *m, std::string animationFilePath)
    : TickerScreen(m)
{
    if (!_fileExists(animationFilePath)) {
        // Try resources directory
        std::string resourcesPath = std::string(RESOURCES_DIR) + animationFilePath;
        if (_fileExists(resourcesPath)) {
            animationFilePath = resourcesPath;
        }
    }

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

        SpriteAnimationFrame *animFrame = new SpriteAnimationFrame(frameCanvas, image.animationDelay());
        _frames.push_back(unique_ptr<SpriteAnimationFrame>(animFrame));
    }
}

void SpriteAnimationScreen::Run()
{
    int currentFrameIndex = 0;
    while (running()) {
        SpriteAnimationFrame *nextFrame = _frames[currentFrameIndex].get();

        MatrixFrame *frame = nextFrame->frame;
        _matrix->SwapOnVSync(frame);

        currentFrameIndex = (currentFrameIndex + 1) % _frames.size();
        usleep(nextFrame->delay * 10000);
    }

    MatrixFrame *blankFrame = _matrix->CreateFrameCanvas();
    blankFrame->Clear();
    _matrix->SwapOnVSync(blankFrame);
}
