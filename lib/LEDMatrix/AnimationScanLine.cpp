#include <AnimationScanLine.h>

bool AnimationScanLine::update() {
    if (!Animation::update()) return false;

    uint8_t middle = this->display->getRows() / 2;
    bool oddMiddle = false;
    if (this->display->getRows() % 2 != 0) {
        middle++;
        oddMiddle = true;
    }

    this->display->clear(false);

    uint32_t color = this->display->getStaticColor();
    for (int16_t col = 0; col < this->display->getCols(); col++) {
        this->display->setPixel(col + 1, (middle) - this->step, color, false);
        this->display->setPixel(col + 1, (middle) + this->step, color, false);
    }

    if (this->direction)
        this->step++;
    else
        this->step--;

    if (this->step > middle || (oddMiddle && this->step >= middle)) {
        this->direction = false;
        this->step = middle;
        if (oddMiddle) this->step = middle - 1;
    }
    if (this->step < 0) {
        this->direction = true;
        this->step = 0;
    }

    this->display->update();
    return true;
}

void AnimationScanLine::reset() {
    Animation::reset();
    this->updateEvery = 10;
    this->step = 0;
    this->direction = true;
}

const char* AnimationScanLine::getName() {
    return "Scan Line";
}