#include "common.h"

#pragma once


#define MODIFIER 4
#define FIELD_WIDTH (18*MODIFIER)
#define FIELD_HEIGHT (20*MODIFIER)

void setup_field();

void set_field_value(uint32_t value, int x, int y);

uint32_t get_field_value(int x, int y);

void show_field();

void set_field_zero();