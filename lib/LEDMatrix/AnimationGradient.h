#ifndef LEDMATRIX_ANIMATION_GRADIENT_h
#define LEDMATRIX_ANIMATION_GRADIENT_h

#include <Animation.h>

class AnimationGradient : public Animation {
   public:
    bool update();
    void reset();
    const char* getName();

   private:
    uint8_t saturation = 255;
    uint32_t colorPercentage = 0;
};
#endif