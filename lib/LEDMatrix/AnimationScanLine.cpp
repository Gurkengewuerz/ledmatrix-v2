#include <AnimationScanLine.h>

bool AnimationScanLine::update() {
    if (!Animation::update()) return false;

    uint8_t middle = this->display->getRows() / 2;
    if (this->display->getRows() % 2 != 0) middle++;

    this->display->clear(false);
    
    uint32_t color = this->display->getStaticColor();
    for (int16_t col = 0; col < this->display->getCols(); col++) {
        this->display->setPixel(col, (middle + 1) - step, color, false);
        this->display->setPixel(col, (this->display->getRows() - middle) + step, color, false);
    }

    this->step++;
    if (this->step >= middle) this->step = 0;

    this->display->update();
    return true;
}

void AnimationScanLine::reset() {
    Animation::reset();
    this->updateEvery = 20;
    this->step = 0;
}

const char* AnimationScanLine::getName() {
    return "Scan Line";
}