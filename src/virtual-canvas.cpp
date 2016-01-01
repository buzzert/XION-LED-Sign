#include "virtual-canvas.h"

#include <iostream>
#include <unistd.h>

#define LED_SIZE 6
#define LED_MARGIN 6

using namespace std;

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
    lock_guard<mutex> guard(_currentFrameMutex);
    VirtualFrameCanvas *previous = _currentFrame;
    _currentFrame = other;

    return previous;
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
    using namespace Utils;
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

        {
            lock_guard<mutex> guard(_currentFrameMutex);

            for (unsigned int y = 0; y < height(); y++) {
                for (unsigned int x = 0; x < width(); x++) {
                    Pixel colorVal = *(ValueAt(x, y));

                    SDL_SetRenderDrawColor(_renderer, colorVal.red, colorVal.green, colorVal.blue, 0xFF);
                    SDL_RenderFillRect(_renderer, &rectangle);
                    rectangle.x += LED_SIZE + LED_MARGIN;
                }

                rectangle.y += LED_SIZE + LED_MARGIN;
                rectangle.x = LED_MARGIN;
            }

            SDL_RenderPresent( _renderer );
        }
        
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

Utils::Pixel *VirtualCanvas::ValueAt(int x, int y) const
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
    _framebuffer = (Utils::Pixel *)malloc(sizeof(Utils::Pixel) * height * width);
}

VirtualFrameCanvas::~VirtualFrameCanvas()
{
    delete _framebuffer;
}

void VirtualFrameCanvas::SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue)
{
    Utils::Pixel pixel(red, green, blue);

    Utils::Pixel *val = ValueAt(x, y);
    (*val) = pixel;
}

void VirtualFrameCanvas::Clear()
{
    memset(_framebuffer, 0, sizeof(Utils::Pixel) * (height() * width()));
}

void VirtualFrameCanvas::Fill(uint8_t red, uint8_t green, uint8_t blue)
{

}

Utils::Pixel *VirtualFrameCanvas::ValueAt(int x, int y) const
{
    return &_framebuffer[ (y * width()) + x ];
}
