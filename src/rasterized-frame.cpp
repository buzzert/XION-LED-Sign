#include "rasterized-frame.h"

void RasterizedFrame::DrawLayerAtPoint(Canvas *c, const Utils::Point<> &p) const
{
    for (int y = 0; y < height(); y++) {
        for (int x = 0; x < width(); x++) {
            int relX = p.x + x;
            int relY = p.y + y;
            if (relX >= 0 && relX < c->width() && relY >= 0 && relY < c->height()) {
                Utils::Pixel *pixel = ValueAt(x, y);
                
                if (pixel->alpha == 255 && opacity == 1.0) {
                    // Fully opaque - direct drawing
                    c->SetPixel(relX, relY, pixel->red, pixel->green, pixel->blue);
                } else if (pixel->alpha > 0 && opacity > 0) {
                    // Get current canvas color by reading from our own buffer
                    Utils::Pixel *dest = ((VirtualFrameCanvas*)c)->ValueAt(relX, relY);
                    
                    // Alpha blending
                    float alpha_factor = (pixel->alpha / 255.0f) * opacity;
                    uint8_t new_r = pixel->red * alpha_factor + dest->red * (1 - alpha_factor);
                    uint8_t new_g = pixel->green * alpha_factor + dest->green * (1 - alpha_factor);
                    uint8_t new_b = pixel->blue * alpha_factor + dest->blue * (1 - alpha_factor);
                    
                    c->SetPixel(relX, relY, new_r, new_g, new_b);
                }
            }
        }
    }
}

void RasterizedFrame::DrawLayer(Canvas *c) const
{
    if (!hidden) {
        DrawLayerAtPoint(c, position);
    }
}
