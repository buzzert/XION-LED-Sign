#ifndef RASTERIZED_FRAME_H
#define RASTERIZED_FRAME_H

#include "utils.h"
#include "virtual-canvas.h"

class RasterizedFrame : public VirtualFrameCanvas {
public:
    RasterizedFrame(int width, int height)
        : VirtualFrameCanvas(width, height), position(0, 0) {};

    Utils::Point<> position;
    bool hidden = false;

    void DrawLayer(Canvas *c) const;
    void DrawLayerAtPoint(Canvas *c, const Utils::Point<> &p) const;
};

#endif
