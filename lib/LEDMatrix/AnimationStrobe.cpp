#include <AnimationStrobe.h>

bool AnimationStrobe::update() {
    if (!Animation::update()) return false;

    RgbColor color = RgbColor(0, 0, 0);
    if (this->strobeStatus) color = RgbColor(255, 255, 255);
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