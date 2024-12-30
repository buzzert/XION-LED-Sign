#ifndef UTILS_H
#define UTILS_H

#include "canvas.h"
#include <cstdint>
#include <Magick++.h>
#include "graphics.h"
#include <giomm.h>
#include "resources.h"

namespace Utils {

class Pixel {
public:
    Pixel(uint8_t r, uint8_t g, uint8_t b) : red(r), green(g), blue(b) {};

    uint8_t red;
    uint8_t green;
    uint8_t blue;
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

inline std::string get_resource_path(const std::string& resource_name) {
    return "/net/buzzert/xionled/resources/" + resource_name;
}

inline Magick::Blob resource_blob(const std::string& resource_name) {
    const std::string normalized_path = get_resource_path(resource_name);

    try {
        auto global_resources = Glib::wrap(resources_get_resource(), true);
        auto resource = global_resources->lookup_data(normalized_path);
        if (!resource) {
            throw std::runtime_error("Resource not found: " + resource_name);
        }

        gsize size = resource->get_size();
        gconstpointer data = resource->get_data(size);

        if (!data || size == 0) {
            throw std::runtime_error("Resource is empty: " + resource_name);
        }

        return Magick::Blob(data, size);
    } catch (const Glib::Error& e) {
        throw std::runtime_error("Failed to load resource: " + e.what());
    }
}

} // namespace Utils

#endif
