#include "xaibatsu.h"
#include "graphics.h"
#include "rasterized-frame.h"

#include <iostream>

using namespace std;
using namespace rgb_matrix;

static const Color XAIBATSU_COLOR(12, 255, 100);

Xaibatsu::Xaibatsu(Utils::Size canvasSize)
    : TickerScreen(canvasSize)
{
    Font font;
    const char *fontPath = RESOURCES_DIR "fonts/4x6.bdf";
    cout << "fontPath: " << fontPath << endl;
    font.LoadFont(fontPath);

    std::cout << "canvasSize: " << canvasSize.width << " " << canvasSize.height << std::endl;
    int width = Utils::WidthOfTextWithFont("Xaibatsu", font);
    _labelLayer = unique_ptr<RasterizedFrame>(new RasterizedFrame(width, font.height()));
    _labelLayer->FillAlpha(0, 0, 0, 0);
    DrawText(_labelLayer.get(), font, 0, font.baseline(), XAIBATSU_COLOR, "XAIBATSU");

    _backgroundLayer = unique_ptr<RasterizedFrame>(new RasterizedFrame(canvasSize.width, canvasSize.height));
    _backgroundLayer->FillAlpha(0, 0, 0, 0);
    _backgroundLayer->Clear();
}

void Xaibatsu::Start()
{
    TickerScreen::Start();
}

void Xaibatsu::Update(double timeDelta)
{
    _backgroundOffset += 1.5;

    _cursorBlinkTime += 0.1;
    if (_cursorBlinkTime > 1.0) {
        _cursorVisible = !_cursorVisible;
        _cursorBlinkTime = 0;
    }
}

void Xaibatsu::Draw(MatrixFrame *m)
{
    m->Clear();
    m->Fill(0, 0, 0);

    const float bgopacity = 0.5;
    for (int i = 0; i < canvasSize.height; i++) {
        for (int j = 0; j < canvasSize.width; j++) {
            float xpos = (float)j; float xmax = (float)canvasSize.width;
            float ypos = (float)i; float ymax = (float)canvasSize.height;

            float hue = (xpos / (xmax * 1.5)) * 360;

            float bri = 1.0;
            float yfade = ymax / 3;
            if (ypos < yfade) {
                bri = 1.0 - (ypos / yfade);
                hue = fmod(hue + _backgroundOffset, 360);
            } else  {
                bri = MAX(0.0, (ypos - (ymax - yfade)) / yfade);
                hue = fmod(hue - _backgroundOffset, -360) * -1;
            }

            float r, g, b;
            Utils::HSVtoRGB(&r, &g, &b, hue, 1.0, bri * bgopacity);

            Utils::Pixel pixel((uint8_t)(0xFF * r), (uint8_t)(0xFF * g), (uint8_t)(0xFF * b));
            m->SetPixel(j, i, pixel.red, pixel.green, pixel.blue);
        }
    }

    int labelX = 0;
    int labelY = (canvasSize.height - _labelLayer->height()) / 2;
    _labelLayer->DrawLayerAtPoint(m, Utils::Point<>(labelX, labelY));

    // Draw blinking cursor by inverting colors from _labelLayer
    if (_cursorVisible) {
        const int cursorWidth = 5;
        const int cursorHeight = _labelLayer->height() + 1;
        const int cursorX = _labelLayer->width() - cursorWidth;
        const int cursorY = labelY - 1 + (_labelLayer->height() - cursorHeight) / 2;
        for (int i = 0; i < cursorHeight; i++) {
            for (int j = 0; j < cursorWidth; j++) {
                int relX = cursorX + j;
                int relY = cursorY + i;
                Utils::Pixel *pixel = _labelLayer->ValueAt(cursorX + j, i - 1);
                if (pixel->alpha > 0) {
                    m->SetPixel(relX, relY, 0, 0, 0);
                } else {
                    m->SetPixel(relX, relY, XAIBATSU_COLOR.r, XAIBATSU_COLOR.g, XAIBATSU_COLOR.b);
                }
            }
        }
    }
}
