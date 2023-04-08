#include <AnimationSinus.h>

bool AnimationSinus::update() {
    if (!Animation::update()) return false;

    this->display->clear(false);

    uint8_t middle = this->display->getRows() / 2;

    uint32_t color = this->display->getStaticColor();
    for (int16_t col = 0; col < this->display->getCols(); col++) {
        int8_t height = constrain((sin((col + this->step) * 0.15f) * middle) + middle, 1, this->display->getRows());
        this->display->setPixel(col + 1, height, color, false);
    }

    this->step++;

    this->display->update();
    return true;
}

void AnimationSinus::reset() {
    Animation::reset();
    this->updateEvery = 5;
    this->step = 0;
}

const char* AnimationSinus::getName() {
    return "Sinus Curve";
}