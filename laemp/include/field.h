#include "common.h"

#pragma once


#define FIELD_WIDTH 120
#define FIELD_HEIGHT 80
#define X_PER_I (1.667)
#define Y_PER_I (0.095)
#define CIRCUMFERENCE (19.45)
#define MAX_X (CIRCUMFERENCE)
#define MAX_Y (Y_PER_I * LEDS_COUNT)
#define MUSHING_WIDTH 11
#define MUSHING_HEIGHT 5

void setup_field();

void set_field_value(uint32_t value, int x, int y);

uint32_t get_field_value(int x, int y);

void show_field();

void set_field_zero();

void set_field_color(uint32_t color);