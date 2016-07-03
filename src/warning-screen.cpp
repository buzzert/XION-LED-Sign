#include "warning-screen.h"

#include <iostream>
#include "graphics.h"

using namespace std;
using namespace rgb_matrix;

WarningScreen::WarningScreen(Utils::Size canvasSize)
 : TickerScreen(canvasSize),
   _textAnimation(Color(0xFF, 0x00, 0x00), Color(0x00, 0x00, 0x00), 1.0)
{
    _segmentImage.read(string(RESOURCES_DIR) + "/warning_bar_segment.png");

    _textAnimation.reverses = true;
    _textAnimation.onCompletion = [this](bool reversing) { if (!reversing) { _ToggleText(); } };
}


void WarningScreen::Update(double timeDelta)
{
    _segmentOffset = timeDelta * 15;
    _textAnimation.ApplyDelta(timeDelta);
}

void WarningScreen::Start()
{
    TickerScreen::Start();
    _textAnimation.Reset();
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

void WarningScreen::_ToggleText()
{
    _currentWarningStringIdx = (_currentWarningStringIdx + 1) % _warningStrings.size();
}

void WarningScreen::Draw(MatrixFrame *nextFrame)
{
    int topOffset = _segmentOffset;
    int bottomOffset = -1 * _segmentOffset;

    // Top Bar
    _DrawWarningBarAtPosition(nextFrame, Utils::Point<>(topOffset, 0));

    Font font;
    font.LoadFont((std::string(RESOURCES_DIR) + "/fonts/" + "6x10.bdf").c_str());

    string nextString = _warningStrings[_currentWarningStringIdx];
    const char *nextStringCStr = nextString.c_str();

    int width = Utils::WidthOfTextWithFont(nextStringCStr, font);
    int xPos = (canvasSize.width - width) / 2;

    int height = font.height();
    DrawText(nextFrame, font, xPos, font.baseline() + _segmentImage.rows(), _textAnimation.CurrentValue(), nextStringCStr);

    // Bottom Bar
    _DrawWarningBarAtPosition(nextFrame, Utils::Point<>(bottomOffset, canvasSize.height - _segmentImage.rows()));
}
