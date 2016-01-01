#ifndef MATRIX_TYPE_H
#define MATRIX_TYPE_H

#ifdef USE_VIRTUAL_CANVAS
#include "virtual-canvas.h"
typedef VirtualCanvas      Matrix;
typedef VirtualFrameCanvas MatrixFrame;
#else
#include "led-matrix.h"
typedef rgb_matrix::RGBMatrix    Matrix;
typedef rgb_matrix::FrameCanvas  MatrixFrame;
#endif

#endif
