#ifndef LEDMATRIX_SIMULATION_TYPES_h
#define LEDMATRIX_SIMULATION_TYPES_h

#include <chrono>
#include <experimental/random>

#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x) * (x))

typedef __UINT8_TYPE__ uint8_t;
typedef __INT8_TYPE__ int8_t;
typedef __UINT16_TYPE__ uint16_t;
typedef __INT16_TYPE__ int16_t;
typedef __UINT32_TYPE__ uint32_t;
typedef __INT32_TYPE__ int32_t;

unsigned long millis();

long random(long howsmall, long howbig);

#endif