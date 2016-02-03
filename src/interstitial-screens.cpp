#include "interstitial-screens.h"

#include "transformer.h"
#include <iostream>
#include <algorithm>
#include <climits>
#include <cmath>

using namespace std;
using namespace rgb_matrix;

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

#pragma mark - RainbowRoad

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
        MatrixFrame *frameCanvas = _offscreenFrame;

        int waveOffset = 0;
        for (int y = 0; y < _matrix->height(); y++) {
            for (int x = 0; x < _matrix->width(); x++) {
                int pixelIndex = abs(x - rainbowOffset - waveOffset) % width;
                pixel_t pixel = horizontalPixels[pixelIndex];

                frameCanvas->SetPixel(x, y, pixel.r, pixel.g, pixel.b);
            }

            waveOffset++;
        }

        _offscreenFrame = _matrix->SwapOnVSync(frameCanvas);
        rainbowOffset++;

        usleep(refreshRate());
    }
}

#pragma mark - DDRArrows

DDRArrows::DDRArrows(Matrix *m)
	: TickerScreen(m)
{
	_blueArrowImage.read(string(RESOURCES_DIR) + "/ddr_arrow_blue.png");
	_pinkArrowImage.read(string(RESOURCES_DIR) + "/ddr_arrow_pink.png");

	srand(time(0));
}

void DDRArrows::_GenerateNewArrowPosition()
{
	int height = _matrix->height();

	int x = -1 * _blueArrowImage.size().width();
	int y = 0;
	switch ((rand() % 4)) {
		case 0:
			y = -4;
			break;
		case 1:
			y = 1;
			break;
		case 2:
			y = 5;
			break;
		case 3:
			y = 9;
			break;
	}

	Arrow arrow;
	arrow.position = Utils::Point(x, y);
	arrow.angle = (rand() % 3) * 90;
	arrow.color = (ArrowColor)(rand() % 2);
	_arrows.push_back(arrow);
}

void DDRArrows::Run()
{
	_GenerateNewArrowPosition();

	RotateTransformer rotateTransformer;

	while (running()) {
		MatrixFrame *nextFrame = _offscreenFrame;
		nextFrame->Clear();

		int smallestXPos = INT_MAX;
		vector<int> indiciesToRemove;
		for (auto it = _arrows.begin(); it != _arrows.end(); it++) {
			Utils::Point &point = it->position;
			int angle = it->angle;
			ArrowColor color = it->color;

			rotateTransformer.SetAngle(angle);

			Magick::Image &image = (color == ArrowColor::Blue) ? _blueArrowImage : _pinkArrowImage;

			// Draw arrow
			int dimensions = max(image.size().width(), image.size().height());
			RasterizedFrame arrowFrame(dimensions, dimensions);
			arrowFrame.Clear();
			Utils::DrawImageIntoCanvas(rotateTransformer.Transform(&arrowFrame), image);

			arrowFrame.DrawLayerAtPoint(nextFrame, point);

			// Advance position
			point.x += 1;
			if (point.x > _matrix->width()) {
				indiciesToRemove.push_back(distance(_arrows.begin(), it));
			}

			if (point.x < smallestXPos) {
				smallestXPos = point.x;
			}
		}

		_offscreenFrame = _matrix->SwapOnVSync(nextFrame);

		if (smallestXPos > 6) {
			_GenerateNewArrowPosition();

			// Cleanup offscreen arrow positions
			for (int i = 0; i < indiciesToRemove.size(); i++) {
				int index = indiciesToRemove[i];
				_arrows.erase(_arrows.begin() + index);
			}
		}

		usleep(1000000 / 30);
	}

	_arrows.clear();
}


} // namespace InterstitialScreen
