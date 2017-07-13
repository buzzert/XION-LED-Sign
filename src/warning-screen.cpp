#include "warning-screen.h"

#include <iostream>
#include <sstream>
#include "graphics.h"

using namespace std;
using namespace rgb_matrix;

WarningScreen::WarningScreen(Utils::Size canvasSize)
 : TickerScreen(canvasSize)
{
    _segmentImage.read(string(RESOURCES_DIR) + "/warning_bar_segment.png");

    _scheduleManager.SetCurfewTimeAsString("22:00:00");
}


void WarningScreen::Update(double timeDelta)
{
    _segmentOffset = timeDelta * 15;

    const int labelYPosition = _segmentImage.rows();
    _timeLeftLayer->position.y = labelYPosition;
    _timeLeftLayer->position.x = canvasSize.width - (timeDelta * 22);

    _descriptionLayer->position.y = labelYPosition;
    _descriptionLayer->position.x = _timeLeftLayer->position.x + _timeLeftLayer->width();

    const double kONTime = 0.7;
    const double kOFFTime = 0.3;

    double timeToChange = (_timeLeftLayer->hidden) ? kOFFTime : kONTime;

    double blinkTime = timeDelta - _blinkTimer;
    if (blinkTime > timeToChange) {
        _blinkTimer = timeDelta;
        _timeLeftLayer->hidden = !_timeLeftLayer->hidden;
    }

    // end state -- when the second label scrolls off screen
    if ( (_descriptionLayer->position.x + _descriptionLayer->width()) < 0 ) {
        _running = false;
    }
}

double WarningScreen::duration() const
{
    int minsRemaining = _scheduleManager.MinutesUntilCurfew();
    if (minsRemaining < 30) {
        return -1;
    } else {
        return 0;
    }
}

void WarningScreen::Start()
{
    TickerScreen::Start();

    _blinkTimer = 0.0;

    // update from schedule manager
    int minsRemaining = _scheduleManager.MinutesUntilCurfew();

    if (minsRemaining > 0) {
        ostringstream timeLeftSS;
        timeLeftSS << minsRemaining << " minute";
        if (minsRemaining > 1) {
            timeLeftSS << "s";
        }

        SetTimeRemainingString(timeLeftSS.str());
        SetWarningString(" until DDR curfew");
    } else {
        SetTimeRemainingString("DDR CURFEW ");
        SetWarningString(" out of respect for our neighbors, please stop all dance gaming after 10pm.");
    }

    _RasterizeTypeLayer(_timeLeftLayer, _timeRemainingString, Color(0xFF, 0x00, 0x00));
    _RasterizeTypeLayer(_descriptionLayer, _warningString, Color(0xFF, 0x00, 0xFF));
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

void WarningScreen::_RasterizeTypeLayer(unique_ptr<RasterizedFrame> &layer, const string &text, const Color &color)
{
    Font font;
    font.LoadFont((std::string(RESOURCES_DIR) + "/fonts/" + "6x10.bdf").c_str());

    const char *textCString = text.c_str();

    int width = Utils::WidthOfTextWithFont(textCString, font);
    int height = font.height();
    if (!layer.get()) {
        int width = Utils::WidthOfTextWithFont(text, font);
        auto newLayer = unique_ptr<RasterizedFrame>(new RasterizedFrame(width, height));
        layer.swap(newLayer);
    }

    layer->Clear();

    DrawText(layer.get(), font, 0, font.baseline(), color, textCString);
}

void WarningScreen::Draw(MatrixFrame *nextFrame)
{
    int topOffset = _segmentOffset;
    int bottomOffset = -1 * _segmentOffset;

    // Top Bar
    _DrawWarningBarAtPosition(nextFrame, Utils::Point<>(topOffset, 0));

    // Text Labels
    _timeLeftLayer->DrawLayer(nextFrame);
    _descriptionLayer->DrawLayer(nextFrame);

    // Bottom Bar
    _DrawWarningBarAtPosition(nextFrame, Utils::Point<>(bottomOffset, canvasSize.height - _segmentImage.rows()));
}
