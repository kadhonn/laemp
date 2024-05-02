#include "common.h"

int rainbow_rows_tick = 0;

void setup_rainbow_rows() {
    rainbow_rows_tick = 0;
}

void rainbow_rows() {
    uint j = rainbow_rows_tick % 256;
    for (int i = 0; i < LEDS_COUNT; i++) {
        strip.setLedColorData(i, strip.Wheel((i * 256 / LEDS_COUNT + j) % 256));
    }
    rainbow_rows_tick++;
}