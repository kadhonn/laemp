#include "common.h"

#pragma once


#define MODIFIER 4
#define FIELD_WIDTH (18*MODIFIER)
#define FIELD_HEIGHT (20*MODIFIER)

extern uint32_t field[FIELD_WIDTH][FIELD_HEIGHT];


uint32_t get_led_color(int i);

void show_field();

void set_field_zero();