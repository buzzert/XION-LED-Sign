#include "clock-screen.h"

#include "graphics.h"

#include <ctime>

using namespace rgb_matrix;
using namespace std;

static Font __clockFont;
static bool __didLoadFont;
static Font& _getClockFont()
{
    if (!__didLoadFont) {
        const char *fontPath = RESOURCES_DIR "/fonts/8x13B.bdf";
        __didLoadFont = __clockFont.LoadFont(fontPath);
    }

    return __clockFont;
}

ClockScreen::ClockScreen(Matrix *m)
    : TickerScreen(m) {}

void ClockScreen::_UpdateTimeString()
{
    const size_t kBufferSize = 16;
    time_t now;
    struct tm *timeinfo;
    char buffer[kBufferSize];

    time(&now);
    timeinfo = localtime(&now);

    strftime(buffer, kBufferSize, "%H:%M:%S", timeinfo);
    _timeString = buffer;
}

void ClockScreen::Run()
{
    _UpdateTimeString();
    clock_t lastUpdated = clock();

    MatrixFrame *offscreenFrame = nullptr;
    while (running()) {
        MatrixFrame *nextFrame = offscreenFrame ? : _matrix->CreateFrameCanvas();
        nextFrame->Clear();

        Font &font = _getClockFont();
        DrawText(nextFrame, font, 0, font.baseline() + 2, Color(0xFF, 0x00, 0x00), _timeString.c_str());

        clock_t now = clock();
        double elapsed = double(now - lastUpdated) / CLOCKS_PER_SEC;

        int dotPosition = (int)(elapsed * (nextFrame->width() - 1));
        if (dotPosition > nextFrame->width()) dotPosition = nextFrame->width() - 1;
        nextFrame->SetPixel(dotPosition, nextFrame->height() - 1, 0xFF, 0x00, 0x00);

        offscreenFrame = _matrix->SwapOnVSync(nextFrame);

        if (elapsed > 1.0) {
            _UpdateTimeString();
            lastUpdated = now;
        }

        usleep(refreshRate());
    }

    delete offscreenFrame;
}
