#ifndef INTERSTITIAL_SCREENS_H
#define INTERSTITIAL_SCREENS_H

#include "ticker-screen.h"

namespace InterstitialScreen {

class RainbowRoad : public TickerScreen {
public:
    RainbowRoad(Matrix *m) : TickerScreen(m) {};
    void Run() override;
};


} // namespace InterstitialScreen

#endif
