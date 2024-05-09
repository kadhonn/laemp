#include "field_helper.h"

void setup_hearts() {

}

#define PIC_SIZE 16

int8_t square[PIC_SIZE * PIC_SIZE / 8] =
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
         -1, -1, -1, -1,};

int8_t heart[PIC_SIZE * PIC_SIZE / 8] =
        {28, 60, 62, 126, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -2, 127, -2, 127, -4, 63, -4, 63, -8, 31, -16,
         15, -64, 3, 0, 0,};


void hearts() {

    int x_pos = FIELD_WIDTH / 2;
    int y_pos = FIELD_HEIGHT / 2;
    uint32_t front_color = strip.Wheel(0);
    uint32_t back_color = strip.hsv2rgb(220, 100, 50);
    int x_mod = 2;
    int y_mod = 2;
    int8_t *longs = heart;

    for (int x = 0; x < FIELD_WIDTH; x++) {
        for (int y = 0; y < FIELD_HEIGHT; y++) {
            set_field_value(back_color, x, y);
        }
    }

    for (int pic_y = 0; pic_y < PIC_SIZE; pic_y++) {
        for (int pic_x = 0; pic_x < PIC_SIZE; pic_x++) {
            int n = pic_y * PIC_SIZE + pic_x;
            bool active = (longs[n / 8] & (1 << (n % 8))) != 0;
            if (active) {
                int x_offset = (pic_x - (PIC_SIZE / 2)) * x_mod;
                //flip image because y is different in pic and field format
                int y_offset = (pic_y - (PIC_SIZE / 2)) * -y_mod;
                int field_x = x_pos + x_offset;
                int field_y = y_pos + y_offset;
                for (int x = field_x; x < field_x + x_mod; x++) {
                    for (int y = field_y; y < field_y + y_mod; y++) {
                        set_field_value(front_color, x, y);
                    }
                }
            }
        }
    }
//    Serial.println("BEGIN");
//    for (int y = FIELD_HEIGHT - 1; y > 0; y--) {
//        for (int x = 0; x < FIELD_WIDTH; x++) {
//            if (x == x_pos && y == y_pos) {
//                Serial.print("@");
//            } else if (get_field_value(x, y) == front_color) {
//                Serial.print("#");
//            } else {
//                Serial.print(" ");
//            }
//        }
//        Serial.println("|");
//    }
//    Serial.println();
//    Serial.println();

    show_field();
}
