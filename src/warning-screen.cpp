#include "warning-screen.h"
#include <iostream>

using namespace std;

WarningScreen::WarningScreen(Matrix *m)
 : TickerScreen(m)
 {
     _segmentImage.read(string(RESOURCES_DIR) + "/warning_bar_segment.png");
 }

 void WarningScreen::Run()
 {
     int topOffset = 0;
     int bottomOffset = 0;

     while (running()) {
         MatrixFrame *nextFrame = _offscreenFrame;
         nextFrame->Clear();

         size_t segmentWidth = _segmentImage.columns();

         // Top warning bars
         int modOffset = topOffset % segmentWidth;
         for (int i = modOffset - segmentWidth; i < _matrix->width();) {
             Utils::DrawImageIntoCanvas(nextFrame, _segmentImage, Utils::Point(i, 0));
             i += segmentWidth;
         }

         // Bottom warning bars
         modOffset = bottomOffset % segmentWidth;
         for (int i = modOffset - segmentWidth; i < _matrix->width();) {
             Utils::DrawImageIntoCanvas(nextFrame, _segmentImage, Utils::Point(i, _matrix->height() - _segmentImage.rows()));
             i += segmentWidth;
         }

         topOffset++;
         bottomOffset--;

         _offscreenFrame = _matrix->SwapOnVSync(nextFrame);

         usleep(1000000 / 30);
     }
 }
