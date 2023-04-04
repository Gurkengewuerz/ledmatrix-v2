#include <AnimationSparkle.h>

bool AnimationSparkle::update() {
    if (!Animation::update()) return false;
    uint8_t row = random(1, this->display->getRows() + 1);
    uint8_t col = random(1, this->display->getCols() + 1);
    uint8_t hue = random(0, 255);

    uint8_t hsv[3];
    RGBConverter::hsvToRgb((float)hue / 255.0, 1.0, 1.0, hsv);
    
    this->display->setPixel(col, row, RgbColor(hsv[0], hsv[1], hsv[2]), true);
    return true;
}

void AnimationSparkle::reset() {
    Animation::reset();
    this->updateEvery = 20;
}

const char* AnimationSparkle::getName() {
    return "Sparkle";
}