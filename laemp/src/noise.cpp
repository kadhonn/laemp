#include "field.h"
#include "noise.h"


int noise_z;
double noise_color_offset;

void setup_noise() {
    noise_z = 0;
    noise_color_offset = 0;
}

void noise() {
    noise_z += 2;
    noise_color_offset+=0.1;
    int color_offset=(int)round(noise_color_offset);
    for (int x = 0; x < FIELD_WIDTH; x++) {
        for (int y = 0; y < FIELD_HEIGHT; y++) {
            uint8_t noise = inoise8(x * 20, y * 20, noise_z);
            set_field_value(strip.Wheel((noise + color_offset) & 255), x, y);
        }
    }

    show_field();

}
