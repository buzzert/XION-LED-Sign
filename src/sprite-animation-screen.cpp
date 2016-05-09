#include "sprite-animation-screen.h"

#include "matrix-type.h"
#include "canvas.h"
#include "utils.h"
#include <sys/stat.h>
#include <iostream>

using namespace rgb_matrix;
using namespace std;

static bool _fileExists(std::string filepath)
{
    struct stat buffer;
    return ( stat(filepath.c_str(), &buffer) == 0 );
}

SpriteAnimationScreen::SpriteAnimationScreen(std::string animationFilePath)
    : TickerScreen()
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
}

vector<unique_ptr<SpriteAnimationScreen::SpriteAnimationFrame>> SpriteAnimationScreen::_PreloadedMatrixFrames(Matrix *matrix)
{
    vector<unique_ptr<SpriteAnimationFrame>> preloadedFrames;

    // Convert image frames into canvases
    for (int i = 0; i < _rawImages.size(); i++) {
        Magick::Image image = _rawImages[i];
        MatrixFrame *frameCanvas = matrix->CreateFrameCanvas();

        Utils::DrawImageIntoCanvas(frameCanvas, image);

        SpriteAnimationFrame *animFrame = new SpriteAnimationFrame(frameCanvas, image.animationDelay());
        preloadedFrames.push_back(unique_ptr<SpriteAnimationFrame>(animFrame));
    }

    return preloadedFrames;
}

Magick::Image SpriteAnimationScreen::_GetCurrentImage() const
{
    return _rawImages[_currentFrameIndex];
}

void SpriteAnimationScreen::Update(double timeDelta)
{
    Magick::Image currentImage = _GetCurrentImage();

    double delayInSeconds = (double)(currentImage.animationDelay() / 100.0);
    int nextFrameIndex = (int)(timeDelta / delayInSeconds) % _rawImages.size();
    _currentFrameIndex = nextFrameIndex;
}

void SpriteAnimationScreen::Draw(MatrixFrame *nextFrame)
{
    Magick::Image currentImage = _GetCurrentImage();
    Utils::DrawImageIntoCanvas(nextFrame, currentImage);
}
