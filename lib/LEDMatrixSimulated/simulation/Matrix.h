#ifndef LEDMATRIX_SIMULATION_MATRIX_h
#define LEDMATRIX_SIMULATION_MATRIX_h

#include <ftxui/screen/screen.hpp>
#include <iostream>

using namespace ftxui;

class Matrix {
   public:
    Matrix(uint16_t pixels);
    ~Matrix();
    void begin();
    void show();
    void setPixelColor(uint16_t pixel, uint32_t color);
    void setBrightness(uint8_t brightness);
    static uint32_t Color(uint8_t r, uint8_t g, uint8_t b);
    static uint32_t ColorHSV(uint16_t hue, uint8_t saturation, uint8_t value);

    void setDimensions(uint16_t rows, uint16_t cols);
    void disableDisplay(bool isDisabled);

   private:
    bool disabled = false;
    uint16_t rows;
    uint16_t cols;
    uint32_t* colors = nullptr;

    ftxui::Screen screen = Screen::Create(Dimension::Fixed(0), Dimension::Fixed(0));
};

#endif