#include "field_helper.h"


uint32_t field[WIDTH][HEIGHT];

uint32_t getField(int x, int y);

void show_field() {
    for (int i = 0; i < LEDS_COUNT; i++) {
        strip.setLedColorData(i, get_led_color(i));
    }
}

void set_field_zero() {
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            field[x][y] = 0;
        }
    }
}

uint32_t get_led_color(int i) {
    double unbounded_x = 1.612 * i;
    int rounded_x_division = floor(unbounded_x / (18.85));

    double x = (unbounded_x - rounded_x_division * (18.85)) * MODIFIER;
    double y = (0.1 * i) * MODIFIER;

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
        color = getField(point_x, point_y);
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

uint32_t getField(int x, int y) {
    return field[(x + WIDTH) % WIDTH][constrain(y, 0, HEIGHT - 1)];
}
