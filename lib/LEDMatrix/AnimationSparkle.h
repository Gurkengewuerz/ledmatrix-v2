#ifndef LEDMATRIX_ANIMATION_SPARKLE_h
#define LEDMATRIX_ANIMATION_SPARKLE_h

#include <Animation.h>

class AnimationSparkle : public Animation {
   public:
    bool update();
    void reset();
    const char* getName();

   private:

};
#endif