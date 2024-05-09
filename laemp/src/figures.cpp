#include "figures.h"


int8_t figure_square[PIC_SIZE * PIC_SIZE / 8] =
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
         -1, -1, -1, -1,};

int8_t figure_heart[PIC_SIZE * PIC_SIZE / 8] =
        {28, 60, 62, 126, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -2, 127, -2, 127, -4, 63, -4, 63, -8, 31, -16,
         15, -64, 3, 0, 0,};

void paint_figure(int x_pos, int y_pos,
                  int x_mod, int y_mod,
                  uint32_t color,
                  int8_t *figure) {

    for (int pic_y = 0; pic_y < PIC_SIZE; pic_y++) {
        for (int pic_x = 0; pic_x < PIC_SIZE; pic_x++) {
            int n = pic_y * PIC_SIZE + pic_x;
            bool active = (figure[n / 8] & (1 << (n % 8))) != 0;
            if (active) {
                int x_offset = (pic_x - (PIC_SIZE / 2)) * x_mod;
                //flip image because y is different in pic and field format
                int y_offset = (pic_y - (PIC_SIZE / 2)) * -y_mod;
                int field_x = x_pos + x_offset;
                int field_y = y_pos + y_offset;
                for (int x = field_x; x < field_x + x_mod; x++) {
                    for (int y = field_y; y < field_y + y_mod; y++) {
                        set_field_value(color, x, y);
                    }
                }
            }
        }
    }
}
