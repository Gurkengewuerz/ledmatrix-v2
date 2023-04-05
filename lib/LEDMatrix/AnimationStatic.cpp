#include <AnimationStatic.h>

bool AnimationStatic::update() {
    if (!Animation::update()) return false;

    this->display->setAllPixel(this->display->getStaticColor(), true);

    return true;
}

void AnimationStatic::reset() {
    Animation::reset();
    this->updateEvery = 20;
}

const char* AnimationStatic::getName() {
    return "Static";
}