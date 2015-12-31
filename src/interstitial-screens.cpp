#include "interstitial-screens.h"

namespace InterstitialScreen {

// From: https://www.cs.rit.edu/~ncs/color/t_convert.html
static void HSVtoRGB(float *r, float *g, float *b, float h, float s, float v)
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

void RainbowRoad::Run()
{
    typedef struct {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    } pixel_t;

    int width = _matrix->width() * 2;

    pixel_t *horizontalPixels = (pixel_t *)malloc(sizeof(pixel_t) * width);
    for (int i = 0; i < width; i++) {
        float hue = ((float)i / (float)width) * 360;
        float sat = 1;
        float bri = 1;

        float r, g, b;
        HSVtoRGB(&r, &g, &b, hue, sat, bri);

        pixel_t pixel = { (uint8_t)(0xFF * r), (uint8_t)(0xFF * g), (uint8_t)(0xFF * b) };
        horizontalPixels[i] = pixel;

        _matrix->SetPixel(i, 0, pixel.r, pixel.g, pixel.b);
    }

    int rainbowOffset = 0;
    while (running()) {
        MatrixFrame *frameCanvas = _matrix->CreateFrameCanvas();

        int waveOffset = 0;
        for (int y = 0; y < _matrix->height(); y++) {
            for (int x = 0; x < _matrix->width(); x++) {
                int pixelIndex = abs(x - rainbowOffset - waveOffset) % width;
                pixel_t pixel = horizontalPixels[pixelIndex];

                frameCanvas->SetPixel(x, y, pixel.r, pixel.g, pixel.b);
            }

            waveOffset++;
        }

        _matrix->SwapOnVSync(frameCanvas);
        rainbowOffset++;

        usleep(refreshRate());
    }
}


} // namespace InterstitialScreen
