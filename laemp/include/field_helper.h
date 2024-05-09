#include "common.h"

#pragma once


#define MODIFIER_X 6
#define MODIFIER_Y 4
#define FIELD_WIDTH (20 * MODIFIER_X)
#define FIELD_HEIGHT (20 * MODIFIER_Y)
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