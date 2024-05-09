#include "field.h"

#pragma once

#define PIC_SIZE 16


extern int8_t figure_square[PIC_SIZE * PIC_SIZE / 8];

extern int8_t figure_heart[PIC_SIZE * PIC_SIZE / 8];

void paint_figure(int x_pos, int y_pos,
                  int x_mod, int y_mod,
                  uint32_t color,
                  int8_t *figure);