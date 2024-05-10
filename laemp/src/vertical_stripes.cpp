#include "field.h"

struct VerticalStripesData {
    int tick;
};

VerticalStripesData vertical_stripes_data;

void setup_vertical_stripes() {
    vertical_stripes_data = VerticalStripesData{
            0
    };
}

void vertical_stripes() {
    for (int x = 0; x < FIELD_WIDTH; x++) {
        int mod_x = (vertical_stripes_data.tick / 2 + x) % FIELD_WIDTH;
        uint32_t color = strip.Wheel((byte) (((float) mod_x) / FIELD_WIDTH * 255.0));
        for (int y = 0; y < FIELD_HEIGHT; y++) {
            set_field_value(color, x, y);
        }
    }
    show_field();
    vertical_stripes_data.tick++;
}
