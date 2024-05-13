#include "field.h"
#include "noise.h"


int noise_z;
double noise_color_offset;

void setup_noise() {
    noise_z = 0;
    noise_color_offset = 0;
}

void noise() {
    noise_z += 1;
    noise_color_offset += 0.05;
    int color_offset = (int) round(noise_color_offset);
    for (int x = 0; x < FIELD_WIDTH; x += 2) {
        for (int y = 0; y < FIELD_HEIGHT; y += 2) {
            uint8_t noise = inoise8(x * 20, y * 20, noise_z);
            uint32_t color = strip.Wheel((noise + color_offset) & 255);
            set_field_value(color, x, y);
            set_field_value(color, x + 1, y);
            set_field_value(color, x, y + 1);
            set_field_value(color, x + 1, y + 1);
        }
    }

    show_field();

}
