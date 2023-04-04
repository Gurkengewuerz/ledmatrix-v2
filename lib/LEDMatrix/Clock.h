#ifndef LEDMATRIX_CLOCK_h
#define LEDMATRIX_CLOCK_h

#include <Arduino.h>

class Clock {
   public:
    Clock(float tps, bool startPaused = false);
    float getTPS();
    void setTPS(float tps);
    bool update();
    bool isPaused();
    void start();
    void stop();

   private:
    float tps = 0.0f;
    unsigned long updateEvery = 0;
    unsigned long updateLast = 0;
    bool paused = false;
};
#endif