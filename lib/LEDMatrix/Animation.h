#ifndef LEDMATRIX_ANIMATION_h
#define LEDMATRIX_ANIMATION_h

#include <Arduino.h>
#include <WS281X.h>
#include <RGBConverter.h>

class Animation {
   public:
    void init(WS281X* display);
    void start();
    void stop();
    virtual void reset();
    virtual bool update();
    virtual const char* getName() = 0;

   protected:
    WS281X* display;
    bool running = true;
    uint16_t ticks = 0;
    uint16_t updateEvery = 20;
};
#endif