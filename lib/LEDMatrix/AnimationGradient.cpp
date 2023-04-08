#include <AnimationGradient.h>

bool AnimationGradient::update() {
    if (!Animation::update()) return false;

    const uint8_t colorStep = 1;

    for (int16_t row = 1; row <= this->display->getRows(); row++) {
        uint32_t color = display->getColorHSV(((this->colorPercentage + ((row - 1) * colorStep)) % 255), this->saturation, 255);
        for (int16_t col = 1; col <= this->display->getCols(); col++) {
            this->display->setPixel(col, row, color, false);
        }
    }

    this->display->update();

    this->colorPercentage += colorStep;
    return true;
}

void AnimationGradient::reset() {
    Animation::reset();
    this->updateEvery = 20;
    this->colorPercentage = 0;
}

const char* AnimationGradient::getName() {
    return "Gradient";
}