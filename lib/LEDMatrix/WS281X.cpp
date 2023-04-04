#include <WS281X.h>

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip((const uint16_t)1024 /* led count */, (const uint8_t)0 /* pin */);

bool WS281X::init(uint8_t cols, uint8_t rows, uint8_t pin) {
    this->cols = cols;
    this->rows = rows;
    this->count = cols * rows;

    this->display = new RgbColor*[this->rows];
    for (int row = 0; row < rows; row++) {
        this->display[row] = new RgbColor[this->cols];
    }

    // pin is currently hardcoded
    strip.Begin();

    // this->setMaxBrightness(this->maxBrightness);
    this->clear(true);

    this->setAllPixel(RgbColor(255, 0, 255), true);
    return true;
}

void WS281X::setPixel(uint8_t col, uint8_t row, RgbColor color, bool finished = true) {
    if (col > this->cols || row > this->rows) return;
    this->display[row - 1][col - 1] = color;
    if (finished) this->update();
}

RgbColor* WS281X::getPixel(uint8_t col, uint8_t row) {
    if (col > this->cols || row > this->rows) return 0;
    return &this->display[row - 1][col - 1];
}

void WS281X::setAllPixel(RgbColor color, bool finished = true) {
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
        if (row % 2 == 0) {
            for (int16_t col = 0; col < this->cols; col++) {
                strip.SetPixelColor((row * this->rows) + col, *this->getPixel(row + 1, col + 1));
            }
        } else {
            // reverse every second row
            for (int16_t col = this->cols - 1; col >= 0; col--) {
                strip.SetPixelColor((row * this->rows) + col, *this->getPixel(row + 1, col + 1));
            }
        }
    }
    strip.Show();
}

bool WS281X::isOccupied(uint8_t col, uint8_t row) {
    return this->getPixel(col, row) == 0;
}

uint8_t WS281X::getBrightness() {
    return this->brightness;
}

void WS281X::setBrightness(uint8_t brightness) {
    this->brightness = brightness;
    float adjustedBrightness = ((float)this->maxBrightness / (float)255) * ((float)brightness / (float)255);
    // this->pixels.setBrightness((uint8_t)((float)this->brightness * adjustedBrightness));
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
