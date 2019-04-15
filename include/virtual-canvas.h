#ifndef VIRTUAL_CANVAS_H
#define VIRTUAL_CANVAS_H

#include <condition_variable>
#include <stdint.h>
#include <vector>

#include <thread>
#include <mutex>

#include "led-matrix.h"
#include "utils.h"

#ifdef USE_VIRTUAL_CANVAS
#include "SDL2/SDL.h"

class VirtualFrameCanvas;

class VirtualCanvas : public rgb_matrix::Canvas
{
public:
    VirtualCanvas(int rows = 16, int chainedDisplays = 1, int parallelDisplays = 1);
    ~VirtualCanvas();

    // From rgb_matrix::Canvas
    virtual int width() const;
    virtual int height() const;
    virtual void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);
    virtual void Clear();
    virtual void Fill(uint8_t red, uint8_t green, uint8_t blue);

    VirtualFrameCanvas *CreateFrameCanvas();
    VirtualFrameCanvas *SwapOnVSync(VirtualFrameCanvas *other);

    int simulatedWidth() const;
    int simulatedHeight() const;

    bool runningSimulation();
    void StartSimulation();
    void StopSimulation();

private:
    const int _rows;
    const int _chainedDisplays;
    const int _parallelDisplays;

    VirtualFrameCanvas *_currentFrame;
    std::mutex          _currentFrameMutex;
    std::vector<VirtualFrameCanvas *> _createdFrames;
    std::condition_variable _frameSwapSemaphore;

    SDL_Window   *_window;
    SDL_Renderer *_renderer;
    bool          _runningSimulation;
    std::mutex    _runningSimulationMutex;

    Utils::Pixel *ValueAt(int x, int y) const;
};

#endif

/// This class is used for double-buffering
class VirtualFrameCanvas : public rgb_matrix::Canvas {
public:
    VirtualFrameCanvas(int width, int height);
    ~VirtualFrameCanvas();

    // Canvas methods
    virtual int width() const { return _width; };
    virtual int height() const { return _height; };
    virtual void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);
    virtual void Clear();
    virtual void Fill(uint8_t red, uint8_t green, uint8_t blue);

    Utils::Pixel *ValueAt(int x, int y) const;

private:
    friend class VirtualCanvas;

    int _width;
    int _height;

    Utils::Pixel *_framebuffer;
};

#endif
