#ifndef TEXT_SCREEN_H
#define TEXT_SCREEN_H

#include <string>
#include "ticker-screen.h"
#include "graphics.h"

class TextScreen : public TickerScreen
{
public:
    enum LabelStyle {
        Default,
        Subtitle,
    };

    enum ScrollingStyle {
        WaitThenScroll, // default
        ScrollIn
    };

    // In seconds
    double scrollingSpeed = 0.05;
    double initialPauseDuration = 3.0;

    rgb_matrix::Color titleColor;
    rgb_matrix::Color subtitleColor;

    LabelStyle     labelStyle;
    ScrollingStyle scrollingStyle;

    TextScreen(Matrix *m, LabelStyle labelStyle = LabelStyle::Default);

    const std::string& GetTitleLabel() const { return _titleLabel; };
    void SetTitleLabel(const std::string& label);

    const std::string& GetSubtitleLabel() const { return _subtitleLabel; };
    void SetSubtitleLabel(const std::string& label);

    virtual void Run() override;

private:
    rgb_matrix::Font *_CachedFontWithName(const std::string& name);

    class RasterizedTypeLayer : public VirtualFrameCanvas {
    public:
        RasterizedTypeLayer(int width, int height)
            : VirtualFrameCanvas(width, height), position(0, 0) {};
        Utils::Point position;

        void DrawLayer(Canvas *c) const;
    };

    std::unique_ptr<RasterizedTypeLayer> _titleLayer;
    std::unique_ptr<RasterizedTypeLayer> _subtitleLayer;

    std::string _titleLabel;
    std::string _subtitleLabel;

    void _RasterizeTitleLayer();
    void _RasterizeSubtitleLayer();
};

#endif
