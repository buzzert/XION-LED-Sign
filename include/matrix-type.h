#ifdef USE_VIRTUAL_CANVAS
#include "virtual-canvas.h"
typedef VirtualCanvas Matrix;
#else
#include "led-matrix.h"
typedef RGBMatrix Matrix;
#endif
