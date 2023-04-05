#ifndef LEDMATRIX_WS281X_h
#define LEDMATRIX_WS281X_h
#include <NeoPixelBus.h>
#include <NeoPixelBrightnessBus.h>

class WS281X {
   public:
    bool init(uint8_t cols, uint8_t rows, uint8_t pin);
    void setPixel(uint8_t col, uint8_t row, RgbColor color, bool finished);
    RgbColor* getPixel(uint8_t col, uint8_t row);
    void setAllPixel(RgbColor color, bool finished);
    void clear(bool finished);
    void update();
    bool isOccupied(uint8_t col, uint8_t row);
    uint8_t getBrightness();
    void setBrightness(uint8_t brightness);
    uint8_t getMaxBrightness();
    void setMaxBrightness(uint8_t maxBrightness);
    uint16_t pixelCount();
    uint8_t getRows();
    uint8_t getCols();

    uint8_t cols = 0;
    uint8_t rows = 0;

   private:
    uint16_t count = 0;
    float brightness = 0.5;
    float maxBrightness = 1;
    RgbColor** display;
};
#endif