#ifndef LEDMATRIX_ANIMATION_SINUS_h
#define LEDMATRIX_ANIMATION_SINUS_h

#include <Animation.h>

class AnimationSinus : public Animation {
   public:
    bool update();
    void reset();
    const char* getName();

    private:
    uint32_t step;
};
#endif