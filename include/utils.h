#ifndef UTILS_H
#define UTILS_H

#include "canvas.h"
#include <cstdint>
#include <Magick++.h>

namespace Utils {

class Pixel {
public:
    Pixel(uint8_t r, uint8_t g, uint8_t b) : red(r), green(g), blue(b) {};

    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

class Point {
public:
    Point(int x, int y) : x(x), y(y) {};
    int x;
    int y;
};

static void DrawImageIntoCanvas(rgb_matrix::Canvas *m, const Magick::Image& image, Point origin)
{
    for (size_t y = 0; y < image.rows(); ++y) {
        for (size_t x = 0; x < image.columns(); ++x) {
            const Magick::Color &c = image.pixelColor(x, y);
            if (c.alphaQuantum() == 0) {
                int destX = origin.x + x;
                int destY = origin.y + y;

                if (destX >= 0 && destX < m->width() &&
                    destY >= 0 && destY < m->height()) {
                    m->SetPixel(destX, destY,
                                   ScaleQuantumToChar(c.redQuantum()),
                                   ScaleQuantumToChar(c.greenQuantum()),
                                   ScaleQuantumToChar(c.blueQuantum())
                   );
               }
           }
        }
    }
}

static void DrawImageIntoCanvas(rgb_matrix::Canvas *m, const Magick::Image& image)
{
    DrawImageIntoCanvas(m, image, Point(0, 0));
}



} // namespace Utils

#endif
