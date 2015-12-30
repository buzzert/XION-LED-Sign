#include <stdint.h>
#include <vector>
#include "SDL2/SDL.h"

#include <thread>
#include <mutex>

#include "led-matrix.h"

class VirtualCanvas : public rgb_matrix::Canvas
{
public:
    VirtualCanvas(int rows = 32, int chainedDisplays = 1, int parallelDisplays = 1);
    ~VirtualCanvas();

    // From rgb_matrix::Canvas
    virtual int width() const;
    virtual int height() const;
    virtual void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);
    virtual void Clear();
    virtual void Fill(uint8_t red, uint8_t green, uint8_t blue);

    // Shared methods between VirtualCanvas and RGBMatrix
    rgb_matrix::FrameCanvas *CreateFrameCanvas();
    rgb_matrix::FrameCanvas *SwapOnVSync(rgb_matrix::FrameCanvas *other);

    int simulatedWidth() const;
    int simulatedHeight() const;

    bool runningSimulation();
    void StartSimulation();
    void StopSimulation();

private:
    const int _rows;
    const int _chainedDisplays;
    const int _parallelDisplays;

    SDL_Window   *_window;
    SDL_Renderer *_renderer;
    uint32_t     *_framebuffer;
    bool          _runningSimulation;
    std::mutex    _runningSimulationMutex;

    uint32_t *ValueAt(int x, int y);
};
