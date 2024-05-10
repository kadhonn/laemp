#include "figures.h"

struct HeartsData {
    float x;
    float y;
    int tick;
};

HeartsData hearts_data;

void setup_hearts() {
    hearts_data = HeartsData{
            (float) FIELD_WIDTH / 2.0,
            (float) FIELD_HEIGHT / 2.0,
            0,
    };
}

void hearts() {
    hearts_data.tick++;
    hearts_data.x += 0.1;
    hearts_data.y += sin(hearts_data.tick * 0.02) * 0.1;

    int x_pos = (int) hearts_data.x;
    int y_pos = (int) hearts_data.y;
    uint32_t front_color = strip.Wheel(0);
    uint32_t back_color = strip.hsv2rgb(220, 100, 50);
    int x_size = 4;
    int y_size = 4;
    int8_t *figure = figure_heart;

    set_field_color(back_color);

    paint_figure(x_pos, y_pos, x_size, y_size, front_color, figure);

    show_field();
}
