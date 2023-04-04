#include <Animation.h>

void Animation::init(WS281X* display) {
    this->display = display;
    this->reset();
}

void Animation::start() {
    this->running = true;
}

void Animation::stop() {
    this->running = false;
}

void Animation::reset() {
    this->display->clear(true);
    this->running = true;
    this->ticks = 0;
}

bool Animation::update() {
    if (!this->running) return false;
    this->ticks++;
    if (this->ticks < this->updateEvery) return false;
    this->ticks = 0;
    return true;
}
