#ifndef MATRIX_TYPE_H
#define MATRIX_TYPE_H

#ifdef USE_VIRTUAL_CANVAS
#include "virtual-canvas.h"
typedef VirtualCanvas      Matrix;
typedef VirtualFrameCanvas MatrixFrame;
#else
#include "led-matrix.h"
typedef RGBMatrix    Matrix;
typedef FrameCanvas  MatrixFrame;
#endif

#endif
