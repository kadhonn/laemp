#include "common.h"

int rainbow_solid_tick = 0;

void setup_rainbow_solid() {
    rainbow_solid_tick = 0;
}

void rainbow_solid() {
    uint j = (rainbow_solid_tick / 2) % 256;
    for (int i = 0; i < LEDS_COUNT; i++) {
        strip.setLedColorData(i, strip.Wheel(j));
    }
    rainbow_solid_tick++;
}