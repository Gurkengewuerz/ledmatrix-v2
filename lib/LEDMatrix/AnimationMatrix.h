#ifndef LEDMATRIX_ANIMATION_MATRIX_h
#define LEDMATRIX_ANIMATION_MATRIX_h

#include <Animation.h>
#include <WS281X.h>

class MatrixLine {
   public:
    MatrixLine();
    void init(WS281X* display);
    void tick(uint32_t* buf);
    bool finished();
    bool isValid();

   private:
    bool valid = false;
    uint8_t length = 0;
    uint8_t fadingOutTiles = 0;
    uint8_t closeWhiteColor = 0;
    uint32_t firstTileColor = 0;
    uint8_t row = 0;
    WS281X* display;
};

class AnimationMatrix : public Animation {
   public:
    AnimationMatrix();
    ~AnimationMatrix();
    bool update();
    void reset();
    const char* getName();

   protected:
    void drawLine(uint8_t col);

   private:
    MatrixLine* lines;
    uint32_t* color;
};
#endif