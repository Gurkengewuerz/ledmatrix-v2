#ifndef LEDMATRIX_ANIMATION_SCANLINE_h
#define LEDMATRIX_ANIMATION_SCANLINE_h

#include <Animation.h>

class AnimationScanLine : public Animation {
   public:
    bool update();
    void reset();
    const char* getName();

   private:
   uint8_t step = 0;
};
#endif