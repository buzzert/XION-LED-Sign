#ifndef WARNING_SCREEN_H
#define WARNING_SCREEN_H

#include "ticker-screen.h"
#include "animation.h"
#include <Magick++.h>

class WarningScreen : public TickerScreen
{
public:
    WarningScreen(Utils::Size size);

    std::vector<std::string> GetWarningStrings() const { return _warningStrings; };
    void SetWarningStrings(std::vector<std::string> strings) { _warningStrings = strings; };

    virtual void Start() override;
    virtual void Update(double timeDelta) override;
    virtual void Draw(MatrixFrame *nextFrame) override;

private:
    Magick::Image    _segmentImage;
    int              _segmentOffset;

    std::vector<std::string> _warningStrings;
    unsigned int _currentWarningStringIdx = 0;

    Animation<rgb_matrix::Color> _textAnimation;

    void _DrawWarningBarAtPosition(MatrixFrame *frame, Utils::Point<> position) const;
    void _ToggleText();
};

#endif
