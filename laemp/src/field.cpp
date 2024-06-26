#include "field.h"


uint32_t *field;

int offset_x = (MUSHING_WIDTH - 1) / 2;
int odd_x = (MUSHING_WIDTH - 1) % 2;
int offset_y = (MUSHING_HEIGHT - 1) / 2;
int odd_y = (MUSHING_HEIGHT - 1) % 2;


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

void set_field_value_ywrapping(uint32_t value, int x, int y) {
    int real_x = ((x % FIELD_WIDTH) + FIELD_WIDTH) % FIELD_WIDTH;
    int real_y = ((y % FIELD_HEIGHT) + FIELD_HEIGHT) % FIELD_HEIGHT;
    field[real_y * FIELD_WIDTH + real_x] = value;
}

struct CacheEntry {
    int rounded_x;
    int rounded_y;
};

CacheEntry *cache;

//TODO currently duplicated, too lazy
uint32_t get_led_color_ywrapping(int i) {

    CacheEntry entry = cache[i];

    int rounded_x = entry.rounded_x;
    int rounded_y = entry.rounded_y;

    int xStart = rounded_x - offset_x;
    int xEnd = rounded_x + offset_x + odd_x;
    int yStart = rounded_y - offset_y;
    int yEnd = rounded_y + offset_y + odd_y;

    uint32_t r = 0;
    uint32_t g = 0;
    uint32_t b = 0;
    int count = 0;
    uint32_t color;

    for (int x = xStart; x <= xEnd; x++) {
        int real_x = ((x % FIELD_WIDTH) + FIELD_WIDTH) % FIELD_WIDTH;
        for (int y = yStart; y <= yEnd; y++) {
            int real_y = ((y % FIELD_HEIGHT) + FIELD_HEIGHT) % FIELD_HEIGHT;
            color = field[real_y * FIELD_WIDTH + real_x];
            r += (color >> 16 & 0xFF);
            g += (color >> 8 & 0xFF);
            b += (color & 0xFF);
            count++;
        }
    }
    if (count == 0) {
        return 0;
    }
    r = r / count;
    g = g / count;
    b = b / count;

    return
            (r &255) << 16 | (g&255)<<8|(b&255);
}

uint32_t get_led_color(int i) {

    CacheEntry entry = cache[i];

    int rounded_x = entry.rounded_x;
    int rounded_y = entry.rounded_y;

    int xStart = rounded_x - offset_x;
    int xEnd = rounded_x + offset_x + odd_x;
    int yStart = max(rounded_y - offset_y, 0);
    int yEnd = min(rounded_y + offset_y + odd_y, FIELD_HEIGHT - 1);

    uint32_t r = 0;
    uint32_t g = 0;
    uint32_t b = 0;
    int count = 0;
    uint32_t color;

    for (int x = xStart; x <= xEnd; x++) {
        int real_x = ((x % FIELD_WIDTH) + FIELD_WIDTH) % FIELD_WIDTH;
        for (int y = yStart; y <= yEnd; y++) {
            color = field[y * FIELD_WIDTH + real_x];
            r += (color >> 16 & 0xFF);
            g += (color >> 8 & 0xFF);
            b += (color & 0xFF);
            count++;
        }
    }
    if (count == 0) {
        return 0;
    }
    r = r / count;
    g = g / count;
    b = b / count;

    return
            (r &255) << 16 | (g&255)<<8|(b&255);
}

void show_field_ywrapping() {
    for (int i = 0; i < LEDS_COUNT; i++) {
        strip.setLedColorData(i, get_led_color_ywrapping(i));
    }
}

void show_field() {
    for (int i = 0; i < LEDS_COUNT; i++) {
        strip.setLedColorData(i, get_led_color(i));
    }
}

void set_field_color(uint32_t color) {
    for (int x = 0; x < FIELD_WIDTH; x++) {
        for (int y = 0; y < FIELD_HEIGHT; y++) {
            set_field_value(color, x, y);
        }
    }
}

void set_field_zero() {
    set_field_color(0);
}

void setup_cache() {
    cache = static_cast<CacheEntry *>(malloc(LEDS_COUNT * sizeof(CacheEntry)));
    for (int i = 0; i < LEDS_COUNT; i++) {

        double unbounded_x = X_PER_I * i;
        int rounded_x_division = floor(unbounded_x / CIRCUMFERENCE);

        double real_x = (unbounded_x - rounded_x_division * CIRCUMFERENCE);
        double real_y = (Y_PER_I * i);

        //mapped onto field coordinates
        double field_x = map(real_x, 0, MAX_X, 0, FIELD_WIDTH);
        double field_y = map(real_y, 0, MAX_Y, 0, FIELD_HEIGHT);

        int rounded_x = (int) round(field_x);
        int rounded_y = (int) round(field_y);

        cache[i] = CacheEntry{
                rounded_x, rounded_y
        };
    }
}

void setup_field() {
    field = static_cast<uint32_t *>(malloc(FIELD_HEIGHT * FIELD_WIDTH * sizeof(uint32_t)));
    set_field_zero();

    setup_cache();
}