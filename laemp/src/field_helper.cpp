#include "field_helper.h"


uint32_t *field;

uint32_t get_field_value(int x, int y) {
    int real_x = ((x % FIELD_WIDTH) + FIELD_WIDTH) % FIELD_WIDTH;
    int real_y = constrain(y, 0, FIELD_HEIGHT - 1);
    return field[real_y * FIELD_WIDTH + real_x];
}

void set_field_value(uint32_t value, int x, int y) {
    if (y >= 0 && y < FIELD_HEIGHT) {
        int real_x = ((x % FIELD_WIDTH) + FIELD_WIDTH) % FIELD_WIDTH;
        field[y * FIELD_WIDTH + real_x] = value;
    }
}

uint32_t get_led_color(int i) {
    double unbounded_x = X_PER_I * i;
    int rounded_x_division = floor(unbounded_x / CIRCUMFERENCE);

    double real_x = (unbounded_x - rounded_x_division * CIRCUMFERENCE);
    double real_y = (Y_PER_I * i);

    //mapped onto field coordinates
    double field_x = map(real_x, 0, MAX_X, 0, FIELD_WIDTH);
    double field_y = map(real_y, 0, MAX_Y, 0, FIELD_HEIGHT);

    int rounded_x = round(field_x);
    int rounded_y = round(field_y);

    double r = 0;
    double g = 0;
    double b = 0;
    double totalWeight = 0;
    uint32_t color;
    int offset_x = (MUSHING_WIDTH - 1) / 2;
    int odd_x = (MUSHING_WIDTH - 1) % 2;
    int offset_y = (MUSHING_HEIGHT - 1) / 2;
    int odd_y = (MUSHING_HEIGHT - 1) % 2;
    for (int x = rounded_x - offset_x; x <= rounded_x + offset_x + odd_x; x++) {
        for (int y = rounded_y - offset_y; y <= rounded_y + offset_y + odd_y; y++) {
            if (y >= 0 && y < FIELD_HEIGHT) {
                double weight = pow(MUSHING_HEIGHT + MUSHING_WIDTH, 1) - pow((abs(field_x - x) + abs(field_y - y)), 1);
                color = get_field_value(x, y);
                r += (color >> 16 & 0xFF) * weight;
                g += (color >> 8 & 0xFF) * weight;
                b += (color & 0xFF) * weight;
                totalWeight += weight;
            }
        }
    }
    if (totalWeight == 0) {
        return 0;
    }
    r = r / totalWeight;
    g = g / totalWeight;
    b = b / totalWeight;

    return
            constrain((uint32_t) round(r), 0, 255) << 16
            | constrain((uint32_t) round(g), 0, 255) << 8
            | constrain((uint32_t) round(b), 0, 255);
}

void show_field() {
    for (int i = 0; i < LEDS_COUNT; i++) {
        strip.setLedColorData(i, get_led_color(i));
    }
}

void set_field_color(uint32_t color){
    for (int x = 0; x < FIELD_WIDTH; x++) {
        for (int y = 0; y < FIELD_HEIGHT; y++) {
            set_field_value(color, x, y);
        }
    }
}

void set_field_zero() {
    set_field_color(0);
}

void setup_field() {
    field = static_cast<uint32_t *>(malloc(FIELD_HEIGHT * FIELD_WIDTH * sizeof(uint32_t)));
    set_field_zero();
}