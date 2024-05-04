#include "common.h"

#pragma once


#define MODIFIER 4
#define WIDTH (18*MODIFIER)
#define HEIGHT (20*MODIFIER)

extern uint32_t field[WIDTH][HEIGHT];


uint32_t get_led_color(int i);

void show_field();

void set_field_zero();