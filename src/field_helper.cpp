#include "field_helper.h"


uint32_t *field;

uint32_t get_field_value(int x, int y) {
    int real_x = (x + FIELD_WIDTH) % FIELD_WIDTH;
    int real_y = constrain(y, 0, FIELD_HEIGHT - 1);
    return field[real_y * FIELD_WIDTH + real_x];
}

void set_field_value(uint32_t value, int x, int y) {
    int real_x = (x + FIELD_WIDTH) % FIELD_WIDTH;
    int real_y = constrain(y, 0, FIELD_HEIGHT - 1);
    field[real_y * FIELD_WIDTH + real_x] = value;
}

double map(double value, double from_start, double from_end, double to_start, double to_end) {
    double from_range = from_end - from_start;
    double to_range = to_end - to_start;

    return (value - from_start) / from_range * to_range + to_start;
}

uint32_t get_led_color(int i) {
    double unbounded_x = X_PER_I * i;
    int rounded_x_division = floor(unbounded_x / CIRCUMFERENCE);

    double real_x = (unbounded_x - rounded_x_division * CIRCUMFERENCE);
    double real_y = (Y_PER_I * i);

    //mapped onto field coordinates
    double x = map(real_x, 0, MAX_X, 0, FIELD_WIDTH);
    double y = map(real_y, 0, MAX_Y, 0, FIELD_HEIGHT);

    int x_floor = floor(x);
    int x_ceil = ceil(x);
    int y_floor = floor(y);
    int y_ceil = ceil(y);

    int points[16][2] = {
            {x_floor,     y_floor},
            {x_floor,     y_ceil},
            {x_ceil,      y_floor},
            {x_ceil,      y_ceil},

            {x_floor - 1, y_floor},
            {x_floor - 1, y_ceil},
            {x_ceil + 1,  y_floor},
            {x_ceil + 1,  y_ceil},

            {x_floor,     y_floor - 1},
            {x_floor,     y_ceil + 1},
            {x_ceil,      y_floor - 1},
            {x_ceil,      y_ceil + 1},

            {x_floor - 1, y_floor - 1},
            {x_floor - 1, y_ceil + 1},
            {x_ceil + 1,  y_floor - 1},
            {x_ceil + 1,  y_ceil + 1},
    };

    double r = 0;
    double g = 0;
    double b = 0;
    double totalWeight = 0;
    uint32_t color;
    for (int j = 0; j < 16; j++) {
        int point_x = points[j][0];
        int point_y = points[j][1];
        double weight = pow(4, 2) - pow((abs(point_x - x) + abs(point_y - y)), 2);
        color = get_field_value(point_x, point_y);
        r += (color >> 16 & 0xFF) * weight;
        g += (color >> 8 & 0xFF) * weight;
        b += (color & 0xFF) * weight;
        totalWeight += weight;
    }
    r = r / totalWeight;
    g = g / totalWeight;
    b = b / totalWeight;

    return
            constrain((int) round(r), 0, 255) << 16
            | constrain((int) round(g), 0, 255) << 8
            | constrain((int) round(b), 0, 255);
}

void show_field() {
    for (int i = 0; i < LEDS_COUNT; i++) {
        strip.setLedColorData(i, get_led_color(i));
    }
}

void set_field_zero() {
    for (int x = 0; x < FIELD_WIDTH; x++) {
        for (int y = 0; y < FIELD_HEIGHT; y++) {
            set_field_value(0, x, y);
        }
    }
}

void setup_field() {
    field = static_cast<uint32_t *>(malloc(FIELD_HEIGHT * FIELD_WIDTH * sizeof(uint32_t)));
    set_field_zero();
}