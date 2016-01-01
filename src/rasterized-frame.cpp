#include "rasterized-frame.h"

void RasterizedFrame::DrawLayerAtPoint(Canvas *c, const Utils::Point &p) const
{
    for (int y = 0; y < height(); y++) {
        for (int x = 0; x < width(); x++) {
            int relX = p.x + x;
            int relY = p.y + y;
            if (relX >= 0 && relX < c->width() && relY >= 0 && relY < c->height()) {
                Utils::Pixel *p = ValueAt(x, y);
                c->SetPixel(relX, relY, p->red, p->green, p->blue);
            }
        }
    }
}

void RasterizedFrame::DrawLayer(Canvas *c) const
{
    DrawLayerAtPoint(c, position);
}
