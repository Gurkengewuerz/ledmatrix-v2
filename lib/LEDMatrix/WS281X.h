#ifndef LEDMATRIX_WS281X_h
#define LEDMATRIX_WS281X_h
#include <Adafruit_NeoPixel.h>

class WS281X {
   public:
    bool init(uint8_t cols, uint8_t rows, uint8_t pin);
    void setPixel(uint8_t col, uint8_t row, uint32_t color, bool finished);
    uint32_t* getPixel(uint8_t col, uint8_t row);
    void setAllPixel(uint32_t color, bool finished);
    void setStaticColor(uint32_t color);
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
    uint32_t getStaticColor();
    uint32_t getColor(uint8_t r, uint8_t g, uint8_t b);
    uint32_t getColorHSV(uint8_t hue, uint8_t saturation, uint8_t value);

    uint8_t cols = 0;
    uint8_t rows = 0;

   private:
    uint16_t count = 0;
    uint8_t brightness = 127;
    uint8_t maxBrightness = 255;
    uint32_t** display;
    uint32_t staticColor;
};
#endif