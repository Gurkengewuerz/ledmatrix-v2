#ifndef LEDMATRIX_SIMULATION_MATRIX_h
#define LEDMATRIX_SIMULATION_MATRIX_h

#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>

typedef void (*MatrixRenderCallback)(ftxui::Elements);

class Matrix {
   public:
    Matrix(uint16_t pixels, uint8_t _pin, uint32_t _x);
    ~Matrix();
    void begin();
    void show();
    void setPixelColor(uint16_t pixel, uint32_t color);
    void setBrightness(uint8_t brightness);
    static uint32_t Color(uint8_t r, uint8_t g, uint8_t b);
    static uint32_t ColorHSV(uint16_t hue, uint8_t saturation, uint8_t value);

    void setDimensions(uint16_t rows, uint16_t cols);

    MatrixRenderCallback cb = nullptr;

   private:
    bool disabled = false;
    uint16_t rows;
    uint16_t cols;
    uint32_t* colors = nullptr;
};

#endif