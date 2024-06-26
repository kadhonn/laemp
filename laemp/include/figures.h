#include "field.h"

#pragma once

#define PIC_SIZE 8


extern int8_t figure_heart[PIC_SIZE * PIC_SIZE / 8];

extern int8_t figure_bubble[PIC_SIZE * PIC_SIZE / 8];

void paint_figure(int x_pos, int y_pos,
                  int x_size, int y_size,
                  uint32_t color,
                  int8_t *figure);

void paint_figure_ywrapping(int x_pos, int y_pos,
                  int x_size, int y_size,
                  uint32_t color,
                  int8_t *figure);