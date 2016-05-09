#include "text-screen.h"

#include <time.h>
#include <algorithm>

using namespace rgb_matrix;
using namespace std;

static std::map<std::string, rgb_matrix::Font *> __cachedFonts;

TextScreen::TextScreen(Utils::Size canvasSize, LabelStyle labelStyle)
    : TickerScreen(canvasSize), labelStyle(labelStyle), scrollingStyle(ScrollingStyle::WaitThenScroll),
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
        int width = Utils::WidthOfTextWithFont(_titleLabel, *font);
        int height = font->height();
        _titleLayer = unique_ptr<RasterizedFrame>(new RasterizedFrame(width, height));
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
        int width = Utils::WidthOfTextWithFont(_subtitleLabel, *font);
        int height = font->height();
        _subtitleLayer = unique_ptr<RasterizedFrame>(new RasterizedFrame(width, height));
    }

    _subtitleLayer->Clear();
    DrawText(_subtitleLayer.get(), *font, 0, font->baseline(), subtitleColor, _subtitleLabel.c_str());
}

void TextScreen::Start()
{
    TickerScreen::Start();

    _RasterizeTitleLayer();
    _RasterizeSubtitleLayer();
}

void TextScreen::Update(double timeDelta)
{
    double pauseDuration = initialPauseDuration;
    int initialPosition = 0;
    if (scrollingStyle == ScrollingStyle::ScrollIn) {
        initialPosition = canvasSize.width + 5;
        pauseDuration = 0.0;
    }

    int drawOffset = std::max((int)((timeDelta - pauseDuration) * scrollingSpeed), 0);
    _titleLayer->position.x = initialPosition - drawOffset;
    if (abs(_titleLayer->position.x) > (_titleLayer->width() + 10)) {
        Stop();
    }
}

void TextScreen::Draw(MatrixFrame *nextFrame)
{
    _titleLayer->DrawLayer(nextFrame);
    if (_subtitleLayer) {
        _subtitleLayer->DrawLayer(nextFrame);
    }
}
