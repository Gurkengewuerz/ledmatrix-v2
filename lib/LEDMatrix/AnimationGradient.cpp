#include <AnimationGradient.h>

bool AnimationGradient::update() {
    if (!Animation::update()) return false;

    for (int16_t row = 1; row <= this->display->getRows(); row++) {
        for (int16_t col = 1; col <= this->display->getCols(); col++) {
            uint8_t hsv[3];
            RGBConverter::hsvToRgb(this->colorPercentage + (((float)row / (float)this->display->getRows()) * 0.25f), this->saturation, 1.0f, hsv);
            this->display->setPixel(col, row, RgbColor(hsv[0], hsv[1], hsv[2]), false);
        }
    }

    this->display->update();

    this->colorPercentage += 0.001;
    if (this->colorPercentage > 1.0f) this->colorPercentage = 0.0f;

    return true;
}

void AnimationGradient::reset() {
    Animation::reset();
    this->updateEvery = 20;
    this->colorPercentage = 0.0f;
}

const char* AnimationGradient::getName() {
    return "Gradient";
}