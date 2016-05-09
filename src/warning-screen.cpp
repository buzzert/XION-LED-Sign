#include "warning-screen.h"
#include <iostream>

using namespace std;

WarningScreen::WarningScreen()
 : TickerScreen()
{
    _segmentImage.read(string(RESOURCES_DIR) + "/warning_bar_segment.png");
}


void WarningScreen::Update(double timeDelta)
{
    _segmentOffset = timeDelta * 10;
}

void WarningScreen::Draw(MatrixFrame *nextFrame)
{
    int topOffset = _segmentOffset;
    int bottomOffset = -1 * _segmentOffset;

    size_t segmentWidth = _segmentImage.columns();

    // Top warning bars
    int modOffset = topOffset % segmentWidth;
    for (int i = modOffset - segmentWidth; i < _matrix->width();) {
        Utils::DrawImageIntoCanvas(nextFrame, _segmentImage, Utils::Point<>(i, 0));
        i += segmentWidth;
    }

    // Bottom warning bars
    modOffset = bottomOffset % segmentWidth;
    for (int i = modOffset - segmentWidth; i < _matrix->width();) {
        Utils::DrawImageIntoCanvas(nextFrame, _segmentImage, Utils::Point<>(i, _matrix->height() - _segmentImage.rows()));
        i += segmentWidth;
    }
}
