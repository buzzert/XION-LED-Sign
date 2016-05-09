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

void RainbowRoad::Start()
{
	TickerScreen::Start();

	typedef struct {
		uint8_t r;
		uint8_t g;
		uint8_t b;
	} pixel_t;

	int width = canvasSize.width * 2;

	_horizontalPixels = (Utils::Pixel *)malloc(sizeof(Utils::Pixel) * width);
	for (int i = 0; i < width; i++) {
		float hue = ((float)i / (float)width) * 360;
		float sat = 1;
		float bri = 1;

		float r, g, b;
		HSVtoRGB(&r, &g, &b, hue, sat, bri);

		Utils::Pixel pixel((uint8_t)(0xFF * r), (uint8_t)(0xFF * g), (uint8_t)(0xFF * b));
		_horizontalPixels[i] = pixel;
	}

	_rainbowOffset = 0;
}

void RainbowRoad::Update(double timeDelta)
{
	_rainbowOffset++;
}

void RainbowRoad::Draw(MatrixFrame *nextFrame)
{
	int waveOffset = 0;
	for (int y = 0; y < canvasSize.height; y++) {
		for (int x = 0; x < canvasSize.width; x++) {
			int pixelIndex = abs(x - _rainbowOffset - waveOffset) % (canvasSize.width * 2);
			Utils::Pixel pixel = _horizontalPixels[pixelIndex];

			nextFrame->SetPixel(x, y, pixel.red, pixel.green, pixel.blue);
		}

		waveOffset++;
	}
}

#pragma mark - DDRArrows

DDRArrows::DDRArrows(Utils::Size canvasSize)
	: TickerScreen(canvasSize)
{
	_blueArrowImage.read(string(RESOURCES_DIR) + "/ddr_arrow_blue.png");
	_pinkArrowImage.read(string(RESOURCES_DIR) + "/ddr_arrow_pink.png");

	srand(time(0));
}

void DDRArrows::_GenerateNewArrowPosition()
{
	int height = canvasSize.height;

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
	arrow.position = Utils::Point<double>(x, y);
	arrow.angle = (rand() % 3) * 90;
	arrow.color = (ArrowColor)(rand() % 2);
	_arrows.push_back(arrow);
}

void DDRArrows::Start()
{
	TickerScreen::Start();

	_GenerateNewArrowPosition();
}

void DDRArrows::Update(double timeDelta)
{
	int smallestXPos = INT_MAX;
	vector<int> indiciesToRemove;
	for (auto it = _arrows.begin(); it != _arrows.end(); it++) {
		Utils::Point<double> &point = it->position;

		// Advance position
		point.x += 0.5;

		if (point.x > canvasSize.width) {
			indiciesToRemove.push_back(distance(_arrows.begin(), it));
		}

		if (point.x < smallestXPos) {
			smallestXPos = point.x;
		}
	}

	if (smallestXPos > 6) {
		_GenerateNewArrowPosition();

		// Cleanup offscreen arrow positions
		for (int i = 0; i < indiciesToRemove.size(); i++) {
			int index = indiciesToRemove[i];
			_arrows.erase(_arrows.begin() + index);
		}
	}
}

void DDRArrows::Draw(MatrixFrame *nextFrame)
{
	RotateTransformer rotateTransformer;

	for (auto it = _arrows.begin(); it != _arrows.end(); it++) {
		int                   angle = it->angle;
		ArrowColor    	      color = it->color;
		Utils::Point<double> &point = it->position;

		rotateTransformer.SetAngle(angle);

		Magick::Image &image = (color == ArrowColor::Blue) ? _blueArrowImage : _pinkArrowImage;

		// Draw arrow
		int dimensions = max(image.size().width(), image.size().height());
		RasterizedFrame arrowFrame(dimensions, dimensions);
		arrowFrame.Clear();
		Utils::DrawImageIntoCanvas(rotateTransformer.Transform(&arrowFrame), image);

		Utils::Point<int> pixelPoint(point.x, point.y);
		arrowFrame.DrawLayerAtPoint(nextFrame, pixelPoint);
	}
}

void DDRArrows::Stop()
{
	TickerScreen::Stop();

	_arrows.clear();
}


} // namespace InterstitialScreen
