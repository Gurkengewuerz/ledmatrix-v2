#include <Clock.h>

Clock::Clock(float tps, bool startPaused) {
    this->paused = startPaused;
    this->setTPS(tps);
}

float Clock::getTPS() {
    return this->tps;
}

void Clock::setTPS(float tps) {
    this->tps = tps;
    this->updateEvery = (1.0f / this->tps) * 1000.0f;
}

bool Clock::update() {
    if (this->paused) return false;
    const unsigned long now = millis();
    if (now - this->updateLast < this->updateEvery) return false;
    this->updateLast = now;
    return true;
}
bool Clock::isPaused() {
    return this->paused;
}

void Clock::start() {
    this->paused = false;
}

void Clock::stop() {
    this->paused = true;
}