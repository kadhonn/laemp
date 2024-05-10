#include "figures.h"

int8_t figure_heart[PIC_SIZE * PIC_SIZE / 8] =
        {66, -25, -1, -1, 126, 126, 60, 24,};

int8_t figure_bubble[PIC_SIZE * PIC_SIZE / 8] =
        {24,60,102,-61,-61,102,60,24,};

void paint_figure(int x_pos, int y_pos,
                  int x_size, int y_size,
                  uint32_t color,
                  int8_t *figure) {

    for (int pic_y = 0; pic_y < PIC_SIZE; pic_y++) {
        for (int pic_x = 0; pic_x < PIC_SIZE; pic_x++) {
            int n = pic_y * PIC_SIZE + pic_x;
            bool active = (figure[n / 8] & (1 << (n % 8))) != 0;
            if (active) {
                int x_offset = (pic_x - (PIC_SIZE / 2)) * x_size;
                //flip image because y is different in pic and field format
                int y_offset = (pic_y - (PIC_SIZE / 2)) * -y_size;
                int field_x = x_pos + x_offset;
                int field_y = y_pos + y_offset;
                for (int x = field_x; x < field_x + x_size; x++) {
                    for (int y = field_y; y < field_y + y_size; y++) {
                        set_field_value(color, x, y);
                    }
                }
            }
        }
    }
}
