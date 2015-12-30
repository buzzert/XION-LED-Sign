/*
 * The purpose of this class is to have a way for multiple subclasses of Canvas to
 * implement a few common methods, without changing the Canvas superclass (which is
 * from the rpi-led-matrix library).
 */

 #include <memory>
 #include "canvas.h"
 #include "virtual-canvas.h"

using namespace rgb_matrix;

class CanvasWrapper : public Canvas
{
public:
    CanvasWrapper() {}
    virtual ~CanvasWrapper() {}

    virtual std::shared_ptr<Canvas> canvas() const = 0;

    // From rgb_matrix::Canvas
    virtual int width() const { return canvas()->width(); };
    virtual int height() const { return canvas()->height(); };
    virtual void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) { canvas()->SetPixel(x, y, red, green, blue); };
    virtual void Clear() { canvas()->Clear(); };
    virtual void Fill(uint8_t red, uint8_t green, uint8_t blue) { canvas()->Fill(red, green, blue); };

    // Common existing Canvas methods
    /*
    virtual FrameCanvas *CreateFrameCanvas() { return nullptr; };
    virtual FrameCanvas *SwapOnVSync(FrameCanvas *other) { return nullptr; };
    */

    // Common new Canvas methods
    virtual void Start() {};
};

template<typename T>
class CanvasWrapperSpec : public CanvasWrapper
{
public:
    CanvasWrapperSpec(std::shared_ptr<T> canvas) : _canvas(canvas) {}

    // overrides
    std::shared_ptr<Canvas> canvas() const override { return _canvas; }
    /*
    FrameCanvas *CreateFrameCanvas() override { return _canvas->CreateFrameCanvas(); };
    FrameCanvas *SwapOnVSync(FrameCanvas *other) override { return _canvas->SwapOnVSync(other); };
    */ 

    void Start() override;

private:
    std::shared_ptr<T> _canvas;
};

template<>
void CanvasWrapperSpec<VirtualCanvas>::Start() {
    _canvas->StartSimulation();
}

template<typename T>
void CanvasWrapperSpec<T>::Start() {}
