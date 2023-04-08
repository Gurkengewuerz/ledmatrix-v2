#include <AnimationStrobe.h>

bool AnimationStrobe::update() {
    if (!Animation::update()) return false;

    uint32_t color = display->getColor(0, 0, 0);
    if (this->strobeStatus) color = this->display->getStaticColor();
    this->strobeStatus = !this->strobeStatus;

    this->display->setAllPixel(color, true);

    return true;
}

void AnimationStrobe::reset() {
    Animation::reset();
    this->updateEvery = 2;
    this->strobeStatus = false;
}

const char* AnimationStrobe::getName() {
    return "Strobe";
}