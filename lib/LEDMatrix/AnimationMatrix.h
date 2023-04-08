#ifndef LEDMATRIX_ANIMATION_MATRIX_h
#define LEDMATRIX_ANIMATION_MATRIX_h

#include <Animation.h>
#include <WS281X.h>

class MatrixLine {
   public:
    MatrixLine();
    void init(WS281X* display);
    uint32_t* tick();
    bool finished();
    bool isValid();

   private:
    uint32_t* line;
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
    bool update();
    void reset();
    const char* getName();

   protected:
    void drawLine(uint8_t col, MatrixLine* matrixLine);

   private:
    MatrixLine* lines;
};
#endif