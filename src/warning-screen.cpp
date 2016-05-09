#include "warning-screen.h"
#include <iostream>

using namespace std;

WarningScreen::WarningScreen(Utils::Size canvasSize)
 : TickerScreen(canvasSize)
{
    _segmentImage.read(string(RESOURCES_DIR) + "/warning_bar_segment.png");
}


void WarningScreen::Update(double timeDelta)
{
    _segmentOffset = timeDelta * 15;
}

void WarningScreen::_DrawWarningBarAtPosition(MatrixFrame *frame, Utils::Point<> position) const
{
    size_t segmentWidth = _segmentImage.columns();

    int modOffset = position.x % segmentWidth;
    for (int i = modOffset - segmentWidth; i < canvasSize.width;) {
        Utils::DrawImageIntoCanvas(frame, _segmentImage, Utils::Point<>(i, position.y));
        i += segmentWidth;
    }
}

void WarningScreen::Draw(MatrixFrame *nextFrame)
{
    int topOffset = _segmentOffset;
    int bottomOffset = -1 * _segmentOffset;

    // Top Bar
    _DrawWarningBarAtPosition(nextFrame, Utils::Point<>(topOffset, 0));

    

    // Bottom Bar
    _DrawWarningBarAtPosition(nextFrame, Utils::Point<>(bottomOffset, canvasSize.height - _segmentImage.rows()));
}
