#ifndef UTILS_H
#define UTILS_H

#include "canvas.h"
#include <cstdint>
#include <Magick++.h>
#include "graphics.h"

namespace Utils {

class Pixel {
public:
    Pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) 
        : red(r), green(g), blue(b), alpha(a) {};

    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
};

template<typename T = int>
class Point {
public:
    Point(T x, T y) : x(x), y(y) {};
    T x;
    T y;
};

class Size {
public:
    Size(int width, int height) : width(width), height(height) {};
    int width;
    int height;
};

static void DrawImageIntoCanvas(rgb_matrix::Canvas *m, const Magick::Image& image, Point<int> origin)
{
    for (size_t y = 0; y < image.rows(); ++y) {
        for (size_t x = 0; x < image.columns(); ++x) {
            const Magick::Color &c = image.pixelColor(x, y);
            if (c.quantumAlpha() > 0) {
                int destX = origin.x + x;
                int destY = origin.y + y;

                if (destX >= 0 && destX < m->width() &&
                    destY >= 0 && destY < m->height()) {
                    m->SetPixel(destX, destY,
                                   MagickCore::ScaleQuantumToChar(c.quantumRed()),
                                   MagickCore::ScaleQuantumToChar(c.quantumGreen()),
                                   MagickCore::ScaleQuantumToChar(c.quantumBlue())
                   );
               }
           }
        }
    }
}

static void DrawImageIntoCanvas(rgb_matrix::Canvas *m, const Magick::Image& image)
{
    DrawImageIntoCanvas(m, image, Point<int>(0, 0));
}

static int WidthOfTextWithFont(const std::string& text, const rgb_matrix::Font& font)
{
    int width = 0;
    for (auto it = text.cbegin(); it != text.cend(); it++) {
        char c = *it;
        width += font.CharacterWidth((uint32_t)c);
    }

    return width;
}


// From: https://www.cs.rit.edu/~ncs/color/t_convert.html
inline void HSVtoRGB(float *r, float *g, float *b, float h, float s, float v)
{
	int i;
	float f, p, q, t;
	if (s == 0) {
		// achromatic (grey)
		*r = *g = *b = v;
		return;
	}

	h /= 60;			// sector 0 to 5
	i = floor(h);
	f = h - i;			// factorial part of h
	p = v * (1 - s);
	q = v * (1 - s * f);
	t = v * (1 - s * (1 - f));

	switch (i) {
		case 0:
			*r = v;
			*g = t;
			*b = p;
			break;
		case 1:
			*r = q;
			*g = v;
			*b = p;
			break;
		case 2:
			*r = p;
			*g = v;
			*b = t;
			break;
		case 3:
			*r = p;
			*g = q;
			*b = v;
			break;
		case 4:
			*r = t;
			*g = p;
			*b = v;
			break;
		default:
			*r = v;
			*g = p;
			*b = q;
			break;
	}
}

static void FillRect(rgb_matrix::Canvas *m, int x, int y, int width, int height, const rgb_matrix::Color &color)
{
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            m->SetPixel(x + j, y + i, color.r, color.g, color.b);
        }
    }
}

} // namespace Utils

#endif
