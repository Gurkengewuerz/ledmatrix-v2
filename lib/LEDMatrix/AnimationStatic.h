#ifndef LEDMATRIX_ANIMATION_STATIC_h
#define LEDMATRIX_ANIMATION_STATIC_h

#include <Animation.h>

class AnimationStatic : public Animation {
   public:
    bool update();
    void reset();
    const char* getName();
};
#endif