#ifndef TEXT_SCREEN_H
#define TEXT_SCREEN_H

#include <string>
#include "ticker-screen.h"
#include "graphics.h"
#include "rasterized-frame.h"

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
    double scrollingSpeed = 20;
    double initialPauseDuration = 3.0;

    rgb_matrix::Color titleColor;
    rgb_matrix::Color subtitleColor;

    LabelStyle     labelStyle;
    ScrollingStyle scrollingStyle;

    TextScreen(Utils::Size canvasSize, LabelStyle labelStyle = LabelStyle::Default);

    const std::string& GetTitleLabel() const { return _titleLabel; };
    void SetTitleLabel(const std::string& label);

    const std::string& GetSubtitleLabel() const { return _subtitleLabel; };
    void SetSubtitleLabel(const std::string& label);

    virtual void Start() override;
    virtual void Update(double timeDelta) override;
    virtual void Draw(MatrixFrame *m) override;

private:
    rgb_matrix::Font *_CachedFontWithName(const std::string& name);

    std::unique_ptr<RasterizedFrame> _titleLayer;
    std::unique_ptr<RasterizedFrame> _subtitleLayer;

    std::string _titleLabel;
    std::string _subtitleLabel;

    void _RasterizeTitleLayer();
    void _RasterizeSubtitleLayer();
};

#endif
