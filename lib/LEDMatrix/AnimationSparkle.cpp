#include <AnimationSparkle.h>

bool AnimationSparkle::update() {
    if (!Animation::update()) return false;
    uint8_t row = random(1, this->display->getRows() + 1);
    uint8_t col = random(1, this->display->getCols() + 1);
    uint8_t hue = random(0, 255);

    this->display->setPixel(col, row, display->getColorHSV(hue, 255, 255), true);
    return true;
}

void AnimationSparkle::reset() {
    Animation::reset();
    this->updateEvery = 20;
}

const char* AnimationSparkle::getName() {
    return "Sparkle";
}