#ifndef WARNING_SCREEN_H
#define WARNING_SCREEN_H

#include "ticker-screen.h"
#include "schedule-manager.h"
#include "animation.h"
#include <Magick++.h>
#include "rasterized-frame.h"

class WarningScreen : public TickerScreen
{
public:
    WarningScreen(Utils::Size size);

    void SetTimeRemainingString(const std::string string) { _timeRemainingString = string; };
    void SetWarningString(const std::string string) { _warningString = string; };

    virtual void Start() override;
    virtual void Update(double timeDelta) override;
    virtual void Draw(MatrixFrame *nextFrame) override;
    virtual double duration() const override;

private:
    Magick::Image    _segmentImage;
    int              _segmentOffset;

    std::string      _timeRemainingString;
    std::string      _warningString;

    double _blinkTimer = 0.0;
    std::unique_ptr<RasterizedFrame> _timeLeftLayer;
    std::unique_ptr<RasterizedFrame> _descriptionLayer;

    rgb_matrix::Font _textLayerFont;

    ScheduleManager _scheduleManager;

    void _RasterizeTypeLayer(std::unique_ptr<RasterizedFrame> &layer, const std::string &text, const rgb_matrix::Color &color);
    void _DrawWarningBarAtPosition(MatrixFrame *frame, Utils::Point<> position) const;
};

#endif
