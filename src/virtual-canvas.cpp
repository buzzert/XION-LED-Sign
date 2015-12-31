#include "virtual-canvas.h"

#include <iostream>
#include <unistd.h>

#define LED_SIZE 6
#define LED_MARGIN 6

VirtualCanvas::VirtualCanvas(int rows, int chainedDisplays, int parallelDisplays)
    : _rows(rows), _chainedDisplays(chainedDisplays), _parallelDisplays(parallelDisplays), _runningSimulation(true)
{
    if ( SDL_Init(SDL_INIT_VIDEO) == 0 ) {
        _window = SDL_CreateWindow("LEDTicker", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, simulatedWidth(), simulatedHeight(), SDL_WINDOW_SHOWN);
        if (_window) {
            _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        }
    } else {
        std::cout << "SDL Error: " << SDL_GetError() << "\n";
    }

    _currentFrame = CreateFrameCanvas();
}

VirtualCanvas::~VirtualCanvas()
{
    StopSimulation();
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);

    SDL_Quit();
}

VirtualFrameCanvas *VirtualCanvas::CreateFrameCanvas()
{
    return new VirtualFrameCanvas(width(), height());
}

VirtualFrameCanvas *VirtualCanvas::SwapOnVSync(VirtualFrameCanvas *other)
{
    _currentFrame = other;
    return other;
}

bool VirtualCanvas::runningSimulation()
{
    std::lock_guard<std::mutex> guard(_runningSimulationMutex);
    return _runningSimulation;
}

void VirtualCanvas::StopSimulation()
{
    std::lock_guard<std::mutex> guard(_runningSimulationMutex);
    _runningSimulation = false;
}

void VirtualCanvas::StartSimulation()
{
    while ( runningSimulation() ) {
        SDL_Event e;
        while ( SDL_PollEvent( &e ) != 0 ) {
            if (e.type == SDL_QUIT) {
                StopSimulation();
            }
        }

        SDL_SetRenderDrawColor( _renderer, 0x00, 0x00, 0x00, 0xFF );
        SDL_RenderClear( _renderer );

        SDL_SetRenderDrawColor(_renderer, 0xFF, 0x0, 0x0, 0xFF);

        // Draw LEDs
        SDL_Rect rectangle;
        rectangle.x = LED_MARGIN;
        rectangle.y = LED_MARGIN;
        rectangle.w = rectangle.h = LED_SIZE;

        for (unsigned int y = 0; y < height(); y++) {
            for (unsigned int x = 0; x < width(); x++) {
                uint32_t colorVal = *(ValueAt(x, y));

                uint8_t red = (colorVal >> 16) & 0xFF;
                uint8_t green = (colorVal >> 8) & 0xFF;
                uint8_t blue = colorVal & 0xFF;

                SDL_SetRenderDrawColor(_renderer, red, green, blue, 0xFF);
                SDL_RenderFillRect(_renderer, &rectangle);
                rectangle.x += LED_SIZE + LED_MARGIN;
            }

            rectangle.y += LED_SIZE + LED_MARGIN;
            rectangle.x = LED_MARGIN;
        }

        SDL_RenderPresent( _renderer );
        usleep((1000000 / 60));
    }
}

int VirtualCanvas::simulatedWidth() const
{
    return LED_MARGIN + ( (LED_SIZE + LED_MARGIN) * width() );
}

int VirtualCanvas::simulatedHeight() const
{
    return LED_MARGIN + ( (LED_SIZE + LED_MARGIN) * height() );
}

int VirtualCanvas::width() const
{
    return 32 * _chainedDisplays;
}

int VirtualCanvas::height() const
{
    return _rows;
}

uint32_t *VirtualCanvas::ValueAt(int x, int y)
{
    return _currentFrame->ValueAt(x, y);
}

void VirtualCanvas::SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue)
{
    _currentFrame->SetPixel(x, y, red, green, blue);
}

void VirtualCanvas::Clear()
{
    _currentFrame->Clear();
}

void VirtualCanvas::Fill(uint8_t red, uint8_t green, uint8_t blue)
{
    _currentFrame->Fill(red, green, blue);
}

#pragma mark - VirtualFrameCanvas

VirtualFrameCanvas::VirtualFrameCanvas(int width, int height)
    : _width(width), _height(height)
{
    _framebuffer = (uint32_t *)malloc(sizeof(uint32_t) * height * width);
}

VirtualFrameCanvas::~VirtualFrameCanvas()
{
    delete _framebuffer;
}

void VirtualFrameCanvas::SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue)
{
    uint32_t colorVal = blue;
    colorVal |= (green << 8);
    colorVal |= (red << 16);

    uint32_t *val = ValueAt(x, y);
    (*val) = colorVal;
}

void VirtualFrameCanvas::Clear()
{
    memset(_framebuffer, 0, height() * width());
}

void VirtualFrameCanvas::Fill(uint8_t red, uint8_t green, uint8_t blue)
{

}

uint32_t *VirtualFrameCanvas::ValueAt(int x, int y)
{
    return &_framebuffer[ (y * width()) + x ];
}
