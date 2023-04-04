#ifndef LEDMATRIX_ANIMATION_GRADIENT_h
#define LEDMATRIX_ANIMATION_GRADIENT_h

#include <Animation.h>

class AnimationGradient : public Animation {
   public:
    bool update();
    void reset();
    const char* getName();

   private:
    float saturation = 1.0f;
    float colorPercentage = 0.0f;
};
#endif