#include <stdint.h>
#include <vector>
#include "SDL2/SDL.h"

#include <thread>
#include <mutex>

#include "led-matrix.h"

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

    // Shared methods between VirtualCanvas and RGBMatrix
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

    SDL_Window   *_window;
    SDL_Renderer *_renderer;
    bool          _runningSimulation;
    std::mutex    _runningSimulationMutex;

    uint32_t *ValueAt(int x, int y);
};

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

private:
    friend class VirtualCanvas;

    int _width;
    int _height;

    uint32_t *ValueAt(int x, int y);
    uint32_t *_framebuffer;
};
