#include <WS281X.h>

STIP_TYPE strip(800 /* led count */, (const uint8_t)0 /* pin */, NEO_GRB + NEO_KHZ800);

bool WS281X::init(uint8_t cols, uint8_t rows, uint8_t pin) {
    this->cols = cols;
    this->rows = rows;
    this->count = cols * rows;

    this->display = new uint32_t*[this->rows];
    for (int row = 0; row < rows; row++) {
        this->display[row] = new uint32_t[this->cols];
    }

#ifdef SIMULATION
    strip.setDimensions(rows, cols);
#endif

    // pin is currently hardcoded
    strip.begin();

    this->staticColor = strip.Color(255, 0, 255);

    this->setMaxBrightness(this->maxBrightness);

    this->clear(true);

    return true;
}

void WS281X::setPixel(uint8_t col, uint8_t row, uint32_t color, bool finished = true) {
    if (col > this->cols || row > this->rows) return;
    this->display[row - 1][col - 1] = color;
    if (finished) this->update();
}

uint32_t* WS281X::getPixel(uint8_t col, uint8_t row) {
    if (col > this->cols || row > this->rows) return 0;
    return &this->display[row - 1][col - 1];
}

void WS281X::setAllPixel(uint32_t color, bool finished = true) {
    for (int16_t row = 0; row < this->rows; row++) {
        for (int16_t col = 0; col < this->cols; col++) {
            this->display[row][col] = color;
        }
    }
    if (finished) this->update();
}

void WS281X::clear(bool finished = true) {
    this->setAllPixel(0, finished);
}

void WS281X::update() {
    for (uint8_t row = 0; row < this->rows; row++) {
        const bool isReversed = row % 2 == 0;
        for (int16_t col = 0; col < this->cols; col++) {
            strip.setPixelColor((row * this->rows) + col, *this->getPixel(row + 1, !isReversed ? col + 1 : (this->cols - col)));
        }
    }
    strip.show();
}

bool WS281X::isOccupied(uint8_t col, uint8_t row) {
    return this->getPixel(col, row) == 0;
}

uint8_t WS281X::getBrightness() {
    return this->brightness;
}

void WS281X::setBrightness(uint8_t brightness) {
    this->brightness = brightness;
    float adjustedBrightness = ((float)this->maxBrightness / 255.0f) * ((float)this->brightness / 255.0f);
    strip.setBrightness((uint8_t)(adjustedBrightness * 255.0f));
    this->update();
}

uint8_t WS281X::getMaxBrightness() {
    return this->maxBrightness;
}

void WS281X::setMaxBrightness(uint8_t maxBrightness) {
    this->maxBrightness = maxBrightness;
    // update brightness
    this->setBrightness(this->brightness);
}

uint16_t WS281X::pixelCount() {
    return this->count;
}

uint8_t WS281X::getRows() {
    return this->rows;
}

uint8_t WS281X::getCols() {
    return this->cols;
}

void WS281X::setStaticColor(uint32_t color) {
    this->staticColor = color;
}

uint32_t WS281X::getStaticColor() {
    return this->staticColor;
}

uint32_t WS281X::getColor(uint8_t r, uint8_t g, uint8_t b) {
    return strip.Color(r, g, b);
}

uint32_t WS281X::getColorHSV(uint16_t hue, uint8_t saturation, uint8_t value) {
    return strip.ColorHSV(hue, saturation, value);
}

STIP_TYPE* WS281X::getStrip() {
    return &strip;
}