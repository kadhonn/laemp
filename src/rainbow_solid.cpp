#include "common.h"

int rainbow_solid_tick = 0;

void setup_rainbow_solid() {
    rainbow_solid_tick = 0;
}

void rainbow_solid() {
    byte last_color_index = (rainbow_solid_tick / 4) % 256;
    byte next_color_index = (last_color_index + 1) % 256;
    byte step = rainbow_solid_tick & 0b11;

    int last_color = strip.Wheel(last_color_index);
    int next_color = strip.Wheel(next_color_index);

    byte last_r = last_color >> 16;
    byte last_g = last_color >> 8;
    byte last_b = last_color;

    byte next_r = next_color >> 16;
    byte next_g = next_color >> 8;
    byte next_b = next_color;

    byte current_r = (last_r * (4 - step) + next_r * step) / 4;
    byte current_g = (last_g * (4 - step) + next_g * step) / 4;
    byte current_b = (last_b * (4 - step) + next_b * step) / 4;

    for (int i = 0; i < LEDS_COUNT; i++) {
        strip.setLedColorData(i, current_r, current_g, current_b);
    }
    rainbow_solid_tick++;
}