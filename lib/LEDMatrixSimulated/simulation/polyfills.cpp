
#include "simulation/polyfills.h"

unsigned long millis() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

long random(long howsmall, long howbig) {
    return std::experimental::randint(howsmall, howbig);
}