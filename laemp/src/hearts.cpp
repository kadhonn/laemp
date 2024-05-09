#include "figures.h"

void setup_hearts() {

}

void hearts() {

    int x_pos = FIELD_WIDTH / 2;
    int y_pos = FIELD_HEIGHT / 2;
    uint32_t front_color = strip.Wheel(0);
    uint32_t back_color = strip.hsv2rgb(220, 100, 50);
    int x_mod = 2;
    int y_mod = 2;
    int8_t *figure = figure_heart;

    set_field_color(back_color);

    paint_figure(x_pos, y_pos, x_mod, y_mod, front_color, figure);

    show_field();
}
