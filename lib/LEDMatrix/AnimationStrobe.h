#ifndef LEDMATRIX_ANIMATION_STROBE_h
#define LEDMATRIX_ANIMATION_STROBE_h

#include <Animation.h>

class AnimationStrobe : public Animation {
   public:
    bool update();
    void reset();
    const char* getName();

   private:
      bool strobeStatus = false;
};
#endif