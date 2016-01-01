#ifndef UTILS_H
#define UTILS_H

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

} // namespace Utils

#endif
