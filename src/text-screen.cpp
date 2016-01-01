#include "text-screen.h"

#include <time.h>

using namespace rgb_matrix;
using namespace std;

static std::map<std::string, rgb_matrix::Font *> __cachedFonts;

static int _widthOfTextWithFont(const std::string& text, const Font& font)
{
    int width = 0;
    for (auto it = text.cbegin(); it != text.cend(); it++) {
        char c = *it;
        width += font.CharacterWidth((uint32_t)c);
    }

    return width;
}

TextScreen::TextScreen(Matrix *m, LabelStyle labelStyle)
    : TickerScreen(m), labelStyle(labelStyle), scrollingStyle(ScrollingStyle::WaitThenScroll),
      titleColor(0xFF, 0xFF, 0xFF), subtitleColor(0xFF, 0xFF, 0xFF)
{

}

Font *TextScreen::_CachedFontWithName(const std::string& name)
{
    Font *font = nullptr;
    if (__cachedFonts.find(name) == __cachedFonts.end()) {
        font = new Font();
        if ( font->LoadFont((std::string(RESOURCES_DIR) + "/fonts/" + name).c_str()) ) {
            __cachedFonts[name] = font;
        } else {
            throw std::invalid_argument("unable to find font with name: " + name);
        }
    } else {
        font = __cachedFonts[name];
    }

    return font;
}

void TextScreen::SetTitleLabel(const std::string& label)
{
    _titleLabel = label;
    if (_titleLayer) _RasterizeTitleLayer();
}

void TextScreen::SetSubtitleLabel(const std::string& label)
{
    _subtitleLabel = label;
    if (_subtitleLayer) _RasterizeSubtitleLayer();
}

void TextScreen::_RasterizeTitleLayer()
{
    // Draw into rasterized layer
    std::string fontName;
    if (labelStyle == LabelStyle::Default) {
        fontName = "9x15B.bdf";
    } else if (labelStyle == LabelStyle::Subtitle) {
        fontName = "6x10.bdf";
    }

    Font *font = _CachedFontWithName(fontName);

    if (!_titleLayer) {
        int width = _widthOfTextWithFont(_titleLabel, *font);
        int height = font->height();
        _titleLayer = unique_ptr<RasterizedTypeLayer>(new RasterizedTypeLayer(width, height));
    }

    _titleLayer->Clear();
    DrawText(_titleLayer.get(), *font, 0, font->baseline(), titleColor, _titleLabel.c_str());

    if (labelStyle == LabelStyle::Subtitle) {
        _titleLayer->position.y = 6;
    }
}

void TextScreen::_RasterizeSubtitleLayer()
{
    Font *font = _CachedFontWithName("4x6.bdf");

    if (!_subtitleLayer) {
        int width = _widthOfTextWithFont(_subtitleLabel, *font);
        int height = font->height();
        _subtitleLayer = unique_ptr<RasterizedTypeLayer>(new RasterizedTypeLayer(width, height));
    }

    _subtitleLayer->Clear();
    DrawText(_subtitleLayer.get(), *font, 0, font->baseline(), subtitleColor, _subtitleLabel.c_str());
}

void TextScreen::Run()
{
    _RasterizeTitleLayer();
    _RasterizeSubtitleLayer();

    double pauseDuration = 0;
    if (scrollingStyle == ScrollingStyle::WaitThenScroll) {
        _titleLayer->position.x = 0;
        pauseDuration = initialPauseDuration;
    } else if (scrollingStyle == ScrollingStyle::ScrollIn) {
        _titleLayer->position.x = _matrix->width() + 5;
    }

    int currentFrameIndex = 0;
    MatrixFrame *offscreenFrame = nullptr;
    while (running()) {
        MatrixFrame *nextFrame = offscreenFrame ? : _matrix->CreateFrameCanvas();
        nextFrame->Clear();

        _titleLayer->DrawLayer(nextFrame);
        if (_subtitleLayer) {
            _subtitleLayer->DrawLayer(nextFrame);
        }

        offscreenFrame = _matrix->SwapOnVSync(nextFrame);

        _titleLayer->position.x -= 1;
        if (abs(_titleLayer->position.x) > (_titleLayer->width() + 10)) {
            Stop();
        }

        if (pauseDuration > 0.0) {
            usleep((unsigned int)(pauseDuration * 1000000));
            pauseDuration = 0.0;
        } else {
            usleep((unsigned int)(scrollingSpeed * 1000000));
        }
    }

    delete offscreenFrame;
}

#pragma mark - RasterizedTypeLayer
void TextScreen::RasterizedTypeLayer::DrawLayer(Canvas *c) const
{
    for (int y = 0; y < height(); y++) {
        for (int x = 0; x < width(); x++) {
            int relX = position.x + x;
            int relY = position.y + y;
            if (relX >= 0 && relX < c->width() && relY >= 0 && relY < c->height()) {
                Utils::Pixel *p = ValueAt(x, y);
                c->SetPixel(relX, relY, p->red, p->green, p->blue);
            }
        }
    }
}
