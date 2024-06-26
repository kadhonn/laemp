#include "Freenove_WS2812_Lib_for_ESP32.h"

#define LEDS_COUNT  210
#define LEDS_PIN     16
#define CHANNEL       0

#define FPS 60
#define SAMPLE_LENGTH_MS (1000 * 60 * 3)

#define START_SERVER 1

#pragma once
extern Freenove_ESP32_WS2812 strip;

double map(double value, double from_start, double from_end, double to_start, double to_end);